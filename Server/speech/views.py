from django.shortcuts import render

from .models import User, Speech

def index(request):
  return render(request, "speech/AudioRecorder.html")
  # return render(request, "displayStatus/index.html")
 
 
 
 
 
 