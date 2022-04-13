from django.urls import path
from . import views

app_name = 'speech'
urlpatterns = [
  path('', views.index, name="index") 
]
  
# app_name = 'displayStatus'
# urlpatterns = [
#   path('', views.index, name="index") 
# ]