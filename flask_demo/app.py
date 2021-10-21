from sqlite3.dbapi2 import Connection
from flask import Flask, render_template, request
import sqlite3

app = Flask(__name__)

@app.route("/")
def hello_world():
    return render_template('admin.html')

    
    

@app.route("/addcandidate.html")
def add():
    return render_template('addcandidate.html')

@app.route("/results.html")
def result():
    return render_template('results.html')

if __name__=="__main__":
    app.run(debug=True)
