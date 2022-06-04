from datetime import datetime
from django.db import models

class Audiofile(models.Model):
    name = models.CharField(max_length=255)
    audio = models.FileField(upload_to='audio')
    username = models.CharField(max_length=255, blank=True)
    time_sent = models.DateTimeField(default=datetime.now(), blank=True)

        


class HMM_model_file(models.Model):
    model_name = models.CharField(max_length=255)
    model_id = models.IntegerField()
    username = models.CharField(max_length=255)
    model_file = models.FileField(upload_to='models')
    owner = models.ForeignKey('auth.user', related_name='model', on_delete=models.CASCADE)