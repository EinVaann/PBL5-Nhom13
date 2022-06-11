from turtle import settiltangle
from django.db import models
from django.conf import settings

class Device(models.Model):
    name = models.CharField(max_length=100)
    light = models.BooleanField()
    fan = models.BooleanField()
    music = models.BooleanField()
    user_id = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete=models.CASCADE)
