import sys
import google.generativeai as genai

import requests

genai.configure(api_key="YOUR_API_KEY")  # Replace with your actual API key
model = genai.GenerativeModel('gemini-pro')

def searchsol(text):
    response = model.generate_content(str(text) + " SOLVE OR SUGGESTION (KEEP ANSWERE UNDER 30 CHARACTORS, dont use markdown)")
    return response.text

arg = sys.argv

if str(arg[1]) == '0':
    print("Successfull")
    data = {"text": "Successfull"}
else:
    fi = open("error.txt", 'r')
    res = searchsol(fi.read())

url = "http://192.168.68.250:80/utlpost"
data = {"text": res}

response = requests.post(url, data=data)
