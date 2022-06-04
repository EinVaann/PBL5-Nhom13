import os
import pickle
import re
from xml.parsers.expat import model
from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from django.contrib.auth.forms import UserCreationForm
import numpy as np
from base.models import Audiofile, HMM_model_file
import HMM.recognition as hrg
from django.core.files.base import File
import hmmlearn.hmm as hmm

def home_page(request):
    return render(request, 'authenticate/home.html', {})

def login_user(request):
    if request.method == 'POST':
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(request, username=username, password= password)
        if user is not None:
            login(request, user)
            return redirect('home')
        else:
            messages.success(request, ('There Was An Error'))
            return redirect('login')
    else:
        return render(request, 'authenticate/login.html', {})

def logout_user(request):
    logout(request)
    messages.success(request, ('Logout successfully'))
    return redirect('login')

def register_user(request):
    if request.method == 'POST':
        form = UserCreationForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data['username']
            password = form.cleaned_data['password1']
            user = authenticate(username=username,password= password)
            # add_base_model(username)
            login(request, user)
            messages.success(request, ('Register successfully'))
            return redirect('home')
    else:
        form = UserCreationForm() 
    return render(request, 'authenticate/register.html', {
        'form':form,
        })

def get_activities(request):
    activities = Audiofile.objects.filter(username =request.user)
    if activities is None:
        print("NONE")
    else:
        for activity in activities:
            print(activity.time_sent)
        return render(request, 'authenticate/activities.html', {'activities':activities} )

def add_base_model(request):

    file_name = [i for i in os.listdir('HMM/model') if i.endswith('.pkl')]
    for f,idx in zip(file_name,range(len(file_name))):
        file_paths = os.path.join('HMM/model/',f)
        with open(file_paths, 'rb') as file:
            model_name = re.split('[/ _ .]',file.name)[-2]
            print(model_name,idx,request.user.id)
            base_hmm_model = HMM_model_file.objects.create(model_name = model_name, model_id = idx,
                     username = request.user, model_file= File(file))
            # base_hmm_model.model_file.save('file',file)
    return redirect('home')

def test_model_in_database(request):
    user_model = HMM_model_file.objects.filter(username= request.user)
    model_files = [pickle.load(i.model_file) for i in user_model]
    file_paths = os.path.join('media/audio/recording_7HrOO5o.wav')
    with open(file_paths,'rb') as audioFile:
        test_mfcc = hrg.get_mfcc(audioFile)
        scores = [model_files[i].score(test_mfcc) for i in range(10)]
        print(user_model[int(np.argmax(scores))].model_name)
    return redirect('home')
