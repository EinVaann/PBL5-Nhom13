from django.urls import path
from . import views

urlpatterns = [
    path('view',views.getData, name='views'),
    path('recognition',views.receiveData),
    path('status/<str:pk>/',views.getStatus)
]