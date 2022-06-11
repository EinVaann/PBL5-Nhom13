from datetime import datetime
from django.db import models
from members.models import Device

class Audiofile(models.Model):
    name = models.CharField(max_length=255)
    audio = models.FileField(upload_to='audio')
    device_id = models.ForeignKey(Device, on_delete=models.CASCADE)
    time_sent = models.DateTimeField(default=datetime.now(), blank=True)