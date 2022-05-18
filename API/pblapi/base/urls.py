from django.urls import path
from . import views

urlpatterns = [
    path('view',views.getData),
    path('recognition',views.receiveData),
    path('send',views.receiveESP)
]