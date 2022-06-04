import math

import librosa
import numpy as np
import os
import pickle
from datetime import datetime,date
import requests

model = []
file_name = [i for i in os.listdir('HMM/model') if i.endswith('.pkl')]
for f in file_name:
    file_paths = os.path.join('HMM/model/',f)
    with open(file_paths,'rb') as file:
        model.append(pickle.load(file))   
# ['model_batden.pkl', 'model_batnhac.pkl', 'model_batquat.pkl', 'model_tatden.pkl', 'model_tatnhac.pkl', 'model_tatquat.pkl',
#  'model_xemgio.pkl', 'model_xemngay.pkl', 'model_xemnhietdo.pkl', 'model_xemthoitiet.pkl']
# print(file_name)

def get_model():
    model = []
    file_name = [i for i in os.listdir('HMM/model') if i.endswith('.pkl')]
    for f in file_name:
        file_paths = os.path.join('HMM/model/',f)
        with open(file_paths,'rb') as file:
            model.append(file)
        print(type(file))  
    return model

def get_mfcc(file_path):
    y, sr = librosa.load(file_path)  
    yt , idx = librosa.effects.trim(y, top_db=10)
    hop_length = math.floor(sr * 0.010)  
    win_length = math.floor(sr * 0.025)  
    
    mfcc = librosa.feature.mfcc(
        y=yt, sr=sr, n_mfcc=12, n_fft=1024,
        hop_length=hop_length, win_length=win_length)
    # subtract mean from mfcc --> normalize mfcc
    mfcc = mfcc - np.mean(mfcc, axis=1).reshape((-1, 1))
    # delta feature 1st order and 2nd order
    delta1 = librosa.feature.delta(mfcc, order=1)
    delta2 = librosa.feature.delta(mfcc, order=2)
    # X is 36 x T
    X = np.concatenate([mfcc, delta1, delta2], axis=0) 
    # return T x 36 (transpose of X)
    return X.T  # hmmlearn use T x N matrix

def validate(audioFile):
    test_mfcc = get_mfcc(audioFile)
    scores = [model[i].score(test_mfcc) for i in range(len(file_name))]
    return np.argmax(scores)

def intent(function_index):
    # print(function_index)
    # function_index = 7
    intent = {}
    action = {}
    content = {}

    if function_index>5:
        action['action_id'] = 1
        action['type'] = 'inform'
    else:
        action['action_id'] = 2 
        action['type'] = 'turn'
    if function_index==6:
        content['id'] = 1
        content['type'] = 'information'
        content['name'] = 'time'
        content['main'] = getContent(function_index)
    elif function_index==7:
        content['id'] = 2
        content['type'] = 'information'
        content['name'] = 'date'
        content['main'] = getContent(function_index)
    elif function_index==8:
        content['id'] = 3
        content['type'] = 'information'
        content['name'] = 'temperature'
        content['main'] = getContent(function_index)
    elif function_index==9:
        content['id'] = 4
        content['type'] = 'information' 
        content['name'] = 'weather'
        content['main'] = getContent(function_index)
    if function_index == 1 or function_index == 4:
        content['type'] = 'device'
        content['name'] = 'music'
        content['device_id'] = 3
        content['state'] = (function_index == 1)
    elif function_index == 2 or function_index == 5:
        content['type'] = 'device'
        content['name'] = 'fan'
        content['device_id'] = 2
        content['state'] = (function_index == 2)
    elif function_index == 0 or function_index == 3:
        content['type'] = 'device'
        content['name'] = 'light'
        content['device_id'] = 1
        content['state'] = (function_index == 0)

    intent['status'] = 'success'
    intent['action'] = action
    intent['content'] = content

    print(intent)
    return intent

def getContent(function_index):
    main_content = {}
    if function_index == 6:
        current_time = (str(datetime.now().time())).split('.')[0]
        hour,min,_ = current_time.split(':')
        main_content['hour'] = hour
        main_content['min'] = min
    elif function_index == 7:
        year,month,day = (str(date.today())).split('-')
        main_content['year'] = year
        main_content['month'] = month
        main_content['day'] = day
    elif function_index == 8 or function_index == 9:
        return getWeatherAPI()
    return main_content

def getWeatherAPI():
    main_content = {}
    api_key = "a68e04be06bf02aac6b8325d9ac2a80f"
    base_url = "http://api.openweathermap.org/data/2.5/weather?"
    city_name = 'Danang'
    complete_url = base_url + "appid=" + api_key + "&q=" + city_name
    response = requests.get(complete_url)
    x = response.json()
    if x["cod"] != "404":
        y = x["main"]
        main_content['temperature'] = round(y["temp"] - 273.15)
        z = x["weather"]
        main_content['weather'] = z[0]["main"]
        t = z[0]["id"]
        if t>=200 and t<=232:
            main_content['weather_id'] = 0
        elif t>=300 and t<=321:
            main_content['weather_id'] = 1
        elif t>=500 and t<=531:
            main_content['weather_id'] = 2
        elif t>=600 and t<=622:
            main_content['weather_id'] = 3
        elif t>=701 and t<=781:
            main_content['weather_id'] = 4
        elif t==800:
            main_content['weather_id'] = 5
        elif t>=801 and t<=803:
            main_content['weather_id'] = 6
    else:
        print(" City Not Found ")
    return main_content