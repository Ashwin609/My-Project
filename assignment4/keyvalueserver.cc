#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include "KVCache.cpp"

#include "keyvaluestore.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using kvstore::KeyValueStore;
using kvstore::Request;
using kvstore::Response;


int port,size,policy,poolsize;

class ServerImpl final {
public:
    ~ServerImpl() {
        server_->Shutdown();
        // Always shutdown the completion queue after the server.
        for(auto &i : cq_list){
            i->Shutdown();
        }
    }

    // There is no shutdown handling in this code.
    void Run() {
        std::string server_address("0.0.0.0:"+ std::to_string(port));

        ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "service_" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *asynchronous* service.
        builder.RegisterService(&service_);
        // Get hold of the completion queue used for the asynchronous communication
        // with the gRPC runtime.
        for (auto i = 0; i < poolsize; i++) {
            cq_list.emplace_back(builder.AddCompletionQueue());
        }
        // Finally assemble the server.
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;
        std::vector<std::thread *> threads;
        for (auto i = 0; i < poolsize; i++) {
            threads.emplace_back(new std::thread(&ServerImpl::HandleRpcs, this, i));
        }
        for (const auto &t: threads) {
            t->join();
        }
    }

private:
    // Class encompasing the state and logic needed to serve a request.
    class CallData {
    public:
        // Take in the "service" instance (in this case representing an asynchronous
        // server) and the completion queue "cq" used for asynchronous communication
        // with the gRPC runtime.
        CallData(KeyValueStore::AsyncService* service, ServerCompletionQueue* cq, LRUCache* cache, LFUCache* lfucache)
                : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE), cache(cache), lfucache(lfucache){
            // Invoke the serving logic right away.
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                // Make this instance progress to the PROCESS state.
                status_ = PROCESS;

                // As part of the initial CREATE state, we *request* that the system
                // start processing SayHello requests. In this request, "this" acts are
                // the tag uniquely identifying the request (so that different CallData
                // instances can serve different requests concurrently), in this case
                // the memory address of this CallData instance.
                service_->RequestGetValues(&ctx_, &request_, &responder_, cq_, cq_,
                                           this);

                service_->RequestStorePair(&ctx_, &request_, &responder_, cq_, cq_,
                                           this);

                service_->RequestDeletePair(&ctx_, &request_, &responder_, cq_, cq_,
                                          this);

            } else if (status_ == PROCESS) {
                // Spawn a new CallData instance to serve new clients while we process
                // the one for this CallData. The instance will deallocate itself as
                // part of its FINISH state.
                new CallData(service_, cq_, cache,lfucache);

                // The actual processing.
                if (request_.type() == 1)
                {
                    if(policy == 1)
                    {
                        std::string val = lfucache->get(request_.key());
                        std::cout << "Found Value for : "<< request_.key() << " : " << val << std::endl;
                        reply_.set_value(val);
                    }
                    else{
                        std::string val = cache->get(request_.key());
                        std::cout << "Found Value for : "<< request_.key() << " : " << val << std::endl;
                        reply_.set_value(val);
                    }
                }
                else if (request_.type() == 2)
                {
                    if(policy == 1)
                    {
                        lfucache->put(request_.key(),request_.value());
                        std::cout << "Insertion Successfull : "<<request_.key() << " " << request_.value() << std::endl;
                        reply_.set_value("Insertion Successfull");
                    }
                    else{
                        cache->put(request_.key(),request_.value());
                        std::cout << "Insertion Successfull : "<<request_.key() << " " << request_.value() << std::endl;
                        reply_.set_value("Insertion Successfull");
                    }
                }
                else if (request_.type() == 3)
                {
                    if(policy == 1)
                    {
                       int i =  lfucache->del(request_.key());
                        if(i == 0) {
                            std::cout<<"Deletion For key : "<<request_.key()<<": Successfull"<< std::endl;
                            reply_.set_value("Deletion Successfull");
                        }
                        else if(i==-1) {
                            std::cout<<"Deletion For key : "<<request_.key()<<": File Does not open"<< std::endl;
                            reply_.set_value("File Does not open");
                        }
                        else if(i == -2) {
                            std::cout<<"Deletion For key : "<<request_.key()<<": Key not present in File"<< std::endl;
                            reply_.set_value("Key not present in File");
                        }
                    }
                    else{
                        int i = cache->del(request_.key());
                        if(i == 0) {
                            std::cout<<"Deletion For key : "<<request_.key()<<": Successfull"<< std::endl;
                            reply_.set_value("Deletion Successfull");
                        }
                        else if(i==-1) {
                            std::cout<<"Deletion For key : "<<request_.key()<<": File Does not open"<< std::endl;
                            reply_.set_value("File Does not open");
                        }
                        else if(i == -2) {
                            std::cout<<"Deletion For key : "<<request_.key()<<": Key not present in File"<< std::endl;
                            reply_.set_value("Key not present in File");
                        }
                    }
                }
                // And we are done! Let the gRPC runtime know we've finished, using the
                // memory address of this instance as the uniquely identifying tag for
                // the event.
                status_ = FINISH;
                responder_.Finish(reply_, Status::OK, this);
            } else {
                GPR_ASSERT(status_ == FINISH);
                // Once in the FINISH state, deallocate ourselves (CallData).
                delete this;
            }
        }

    private:
        // The means of communication with the gRPC runtime for an asynchronous
        // server.
        KeyValueStore::AsyncService* service_;
        // The producer-consumer queue where for asynchronous server notifications.
        ServerCompletionQueue* cq_;
        // Context for the rpc, allowing to tweak aspects of it such as the use
        // of compression, authentication, as well as to send metadata back to the
        // client.
        ServerContext ctx_;

        // What we get from the client.
        Request request_;
        // What we send back to the client.
        Response reply_;

        // The means to get back to the client.
        ServerAsyncResponseWriter<Response> responder_;

        // Let's implement a tiny state machine with the following states.
        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;  // The current serving state.

        LRUCache *cache;
        LFUCache *lfucache;
    };

    // This can be run in multiple threads if needed.
    void HandleRpcs(int index) {
        // Spawn a new CallData instance to serve new clients.
        LRUCache *cache = new LRUCache(size);
        LFUCache *lfucache = new LFUCache(size);
        new CallData(&service_, cq_list[index].get(), cache, lfucache);

        void* tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            // Block waiting to read the next event from the completion queue. The
            // event is uniquely identified by its tag, which in this case is the
            // memory address of a CallData instance.
            // The return value of Next should always be checked. This return value
            // tells us whether there is any kind of event or cq_ is shutting down.
            GPR_ASSERT(cq_list[index]->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<CallData*>(tag)->Proceed();
        }
    }

    std::vector<std::unique_ptr<ServerCompletionQueue>> cq_list;
    KeyValueStore::AsyncService service_;
    std::unique_ptr<Server> server_;
};


int main(int argc, char** argv) {
    std::map<std::string, int> config = std::map<std::string, int>();
    std::fstream file;

    file.open("../../config.conf", std::ios::in);
    if (file.is_open()) {
        std::string line, k, v;
        while (getline(file, line)) {
            std::stringstream stream(line);
            getline(stream, k, '=');
            getline(stream, v);
            std::stringstream ss (v);
            int val = 0;
            if(k=="CACHE_REPLACEMENT_TYPE"){
                if (v == "LFU") {
                    val = 1;
                }
                else {
                    val = 2;
                }
            }
            else {
            ss >> val;
            }
            config[k] = val;
        }
        file.close();
    }
    std::map<std::string, int>::iterator itr;
    for (itr = config.begin(); itr != config.end(); itr++)
    {
        if(itr->first=="LISTENING_PORT")
            port = itr->second;
        else if(itr->first=="CACHE_SIZE")
            size = itr->second;
        else if(itr->first=="CACHE_REPLACEMENT_TYPE")
            policy = itr->second;
        else if(itr->first=="THREAD_POOL_SIZE")
            poolsize = itr->second;
    }
    ServerImpl server;
    server.Run();
    return 0;
}