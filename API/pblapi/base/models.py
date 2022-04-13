from django.db import models

class Audiofile(models.Model):
    name = models.CharField(max_length=255)
    audio = models.FileField(upload_to='audio')
