from lib2to3.pgen2 import token
from flask import Flask, request
from flask_restful import Api
from flask_cors import CORS


from api import include_urls

app = Flask(__name__)


api = Api(app)
include_urls(api)

if __name__ == '__main__':
    app.run(debug=True)