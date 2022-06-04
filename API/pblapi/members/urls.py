from django.urls import path
from . import views

urlpatterns = [
    path("", views.home_page, name= 'home'),
    path('login_user', views.login_user, name='login'),
    path('logout_user', views.logout_user, name='logout'),
    path('register', views.register_user, name='register'),
    path('activities', views.get_activities, name='activities'),
    path('test', views.add_base_model, name='test'),
    path('test2', views.test_model_in_database, name='test2'),
]