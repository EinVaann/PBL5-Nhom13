from os import device_encoding
from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from django.contrib.auth.forms import UserCreationForm
from base.models import Audiofile
from .models import Device
from .forms import device_form
def home_page(request):
    return render(request, 'authenticate/home.html', {})

def login_user(request):
    if request.method == 'POST':
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(request, username=username, password= password)
        if user is not None:
            login(request, user)
            print(request.user.id)
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
    devices = Device.objects.filter(user_id = request.user.id)
    activity_log = {}
    for device in devices:
        activities = Audiofile.objects.filter(device_id = device.id)
        activity_log[device.id] = activities
        # for activity in activities:
        #     print(activity.time_sent)
    # print(activity_log)
    # for activities in activity_log.values():
    #     for i in activities:
    #         print(i.time_sent)
    return render(request, 'authenticate/activities.html', {'activity_log':activity_log} )


def get_device(request):
    devices = Device.objects.filter(user_id = request.user.id)
    if devices is None:
        print("NONE")
    else:
        return render(request, 'authenticate/devices.html', {'devices':devices})

def update_device(request,pk):
    device = Device.objects.get(id=pk)
    form = device_form(instance=device)
    if request.method == 'POST':
        form = device_form(request.POST, instance=device)
        if form.is_valid():
            form.save()
            return redirect('/contr_devices/' +pk)
    else:
        return render(request, 'authenticate/control.html', {'form':form})