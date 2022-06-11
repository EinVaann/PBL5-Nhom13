from os import device_encoding
from django.contrib import admin
from .models import Audiofile
from members.models import Device

# Register your models here.
class audio_data(admin.ModelAdmin):
    list_display = ['id','device_id','name','audio','time_sent']
admin.site.register(Audiofile,audio_data)

class device_data(admin.ModelAdmin):
    list_display = ['id','name','light','fan','music','user_id']
admin.site.register(Device, device_data)