#include <iostream>
#include <memory>
#include <string>
#include<bits/stdc++.h>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "keyvaluestore.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using kvstore::KeyValueStore;
using kvstore::Request;
using kvstore::Response;

class KeyValueClient {
public:
    explicit KeyValueClient(std::shared_ptr<Channel> channel)
            : stub_(KeyValueStore::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string GetValues(const std::string& user) {
        // Data we are sending to the server.
        Request request;
        request.set_key(user);
        request.set_type(1);

        // Container for the data we expect from the server.
        Response response;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The producer-consumer queue we use to communicate asynchronously with the
        // gRPC runtime.
        CompletionQueue cq;

        // Storage for the status of the RPC upon completion.
        Status status;

        // stub_->PrepareAsyncSayHello() creates an RPC object, returning
        // an instance to store in "call" but does not actually start the RPC
        // Because we are using the asynchronous API, we need to hold on to
        // the "call" instance in order to get updates on the ongoing RPC.
        std::unique_ptr<ClientAsyncResponseReader<Response> > rpc(
                stub_->PrepareAsyncStorePair(&context, request, &cq));

        // StartCall initiates the RPC call
        rpc->StartCall();

        // Request that, upon completion of the RPC, "reply" be updated with the
        // server's response; "status" with the indication of whether the operation
        // was successful. Tag the request with the integer 1.
        rpc->Finish(&response, &status, (void*)1);
        void* got_tag;
        bool ok = false;
        // Block until the next result is available in the completion queue "cq".
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the cq_ is shutting down.
        GPR_ASSERT(cq.Next(&got_tag, &ok));

        // Verify that the result from "cq" corresponds, by its tag, our previous
        // request.
        GPR_ASSERT(got_tag == (void*)1);
        // ... and that the request was completed successfully. Note that "ok"
        // corresponds solely to the request for updates introduced by Finish().
        GPR_ASSERT(ok);

        // Act upon the status of the actual RPC.
        if (status.ok()) {
            return response.value();
        } else {
            return "RPC failed";
        }
    }
    std::string StorePair(const std::string& p,const std::string& r) {
        // Data we are sending to the server.
        Request request;
        request.set_key(p);
        request.set_value(r);
        request.set_type(2);

        // Container for the data we expect from the server.
        Response response;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The producer-consumer queue we use to communicate asynchronously with the
        // gRPC runtime.
        CompletionQueue cq;

        // Storage for the status of the RPC upon completion.
        Status status;

        // stub_->PrepareAsyncSayHello() creates an RPC object, returning
        // an instance to store in "call" but does not actually start the RPC
        // Because we are using the asynchronous API, we need to hold on to
        // the "call" instance in order to get updates on the ongoing RPC.
        std::unique_ptr<ClientAsyncResponseReader<Response> > rpc(
                stub_->PrepareAsyncStorePair(&context, request, &cq));

        // StartCall initiates the RPC call
        rpc->StartCall();

        // Request that, upon completion of the RPC, "reply" be updated with the
        // server's response; "status" with the indication of whether the operation
        // was successful. Tag the request with the integer 1.
        rpc->Finish(&response, &status, (void*)1);
        void* got_tag;
        bool ok = false;
        // Block until the next result is available in the completion queue "cq".
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the cq_ is shutting down.
        GPR_ASSERT(cq.Next(&got_tag, &ok));

        // Verify that the result from "cq" corresponds, by its tag, our previous
        // request.
        GPR_ASSERT(got_tag == (void*)1);
        // ... and that the request was completed successfully. Note that "ok"
        // corresponds solely to the request for updates introduced by Finish().
        GPR_ASSERT(ok);

        // Act upon the status of the actual RPC.
        if (status.ok()) {
            return response.value();
        } else {
            return "RPC failed";
        }
    }
    std::string DeletePair(const std::string& user) {
        // Data we are sending to the server.
        Request request;
        request.set_key(user);
        request.set_type(3);

        // Container for the data we expect from the server.
        Response response;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The producer-consumer queue we use to communicate asynchronously with the
        // gRPC runtime.
        CompletionQueue cq;

        // Storage for the status of the RPC upon completion.
        Status status;

        // stub_->PrepareAsyncSayHello() creates an RPC object, returning
        // an instance to store in "call" but does not actually start the RPC
        // Because we are using the asynchronous API, we need to hold on to
        // the "call" instance in order to get updates on the ongoing RPC.
        std::unique_ptr<ClientAsyncResponseReader<Response> > rpc(
                stub_->PrepareAsyncStorePair(&context, request, &cq));

        // StartCall initiates the RPC call
        rpc->StartCall();

        // Request that, upon completion of the RPC, "reply" be updated with the
        // server's response; "status" with the indication of whether the operation
        // was successful. Tag the request with the integer 1.
        rpc->Finish(&response, &status, (void*)1);
        void* got_tag;
        bool ok = false;
        // Block until the next result is available in the completion queue "cq".
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the cq_ is shutting down.
        GPR_ASSERT(cq.Next(&got_tag, &ok));

        // Verify that the result from "cq" corresponds, by its tag, our previous
        // request.
        GPR_ASSERT(got_tag == (void*)1);
        // ... and that the request was completed successfully. Note that "ok"
        // corresponds solely to the request for updates introduced by Finish().
        GPR_ASSERT(ok);

        // Act upon the status of the actual RPC.
        if (status.ok()) {
            return response.value();
        } else {
            return "RPC failed";
        }
    }
private:
    // Out of the passed in Channel comes the stub, stored here, our view of the
    // server's exposed services.
    std::unique_ptr<KeyValueStore::Stub> stub_;
};

int main(int argc, char** argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    KeyValueClient client(grpc::CreateChannel(
            "localhost:8081", grpc::InsecureChannelCredentials()));
    /*std::string arg=argv[1];
    if(arg=="GET")
    {
        std::string p = argv[2];
        if(p.length() > 256)
            std::cout<<"key must be less then or equal to 256 characters"<<std::endl;
        else {
            std::string reply = client.GetValues(p);  // The actual RPC call!
            std::cout << p << ":" << reply << std::endl;
        }
    }
    else if(arg == "PUT")
    {
        std::string p = argv[2];
        std::string r = argv[3];
        if(p.length() > 256 || r.length() > 256)
            std::cout<<"key must be less then or equal to 256 characters"<<std::endl;
        else {
            std::string q = client.StorePair(argv[2],argv[3]);
            std::cout << q << std::endl;
        }
    }
    else if(arg == "DEL")
    {
        std::string p = argv[2];
        if(p.length() > 256)
            std::cout<<"key must be less then or equal to 256 characters"<<std::endl;
        else {
            std:: string q = client.DeletePair(argv[2]);
            std::cout<< q << std::endl;
        }
    }*/

    std::vector<std::vector<std::string>> cmds = std::vector<std::vector<std::string>>();
    std::fstream file;
    file.open("../../commands.conf", std::ios::in);
    std::string line, k,v, readcmd;

    while (getline(file, line)) {
        std::vector<std::string> command = std::vector<std::string>();
        std::stringstream stream(line);
        getline(stream, readcmd, ' ');
        getline(stream, k, ' ');
        getline(stream, v);
        command.emplace_back(readcmd);
        command.emplace_back(k);
        command.emplace_back(v);
        cmds.emplace_back(command);
    }
    file.close();
    for(const auto a : cmds){
        std::string arg1 = a.at(0);
        std::string arg2,arg3;
        if(arg1 == "GET")
        {
            arg2 = a.at(1);
            if(arg2.length() > 256)
                std::cout<<"key must be less then or equal to 256 characters"<<std::endl;
            else {
                std::string reply = client.GetValues(arg2);  // The actual RPC call!
                std::cout << arg2 << ":" << reply << std::endl;
            }
        }
        else if(arg1 == "PUT"){
            arg2 = a.at(1);
            arg3 = a.at(2);
            if(arg2.length() > 256 || arg3.length() > 256)
                std::cout<<"key must be less then or equal to 256 characters"<<std::endl;
            else {
                std::string q = client.StorePair(arg2,arg3);
                std::cout << q << std::endl;
            }
        }
        else if(arg1 == "DEL"){
            arg2 = a.at(1);
            if(arg2.length() > 256)
                std::cout<<"key must be less then or equal to 256 characters"<<std::endl;
            else {
                std:: string q = client.DeletePair(arg2);
                std::cout<< q << std::endl;
            }
        }
    }
    return 0;
}