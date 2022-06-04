from django.contrib import admin
from .models import Audiofile, HMM_model_file

# Register your models here.
class audio_data(admin.ModelAdmin):
    list_display = ['id','name','audio','username','time_sent']
admin.site.register(Audiofile,audio_data)

class model_data(admin.ModelAdmin):
    list_display = ['id','model_name','model_id','username','model']
admin.site.register(HMM_model_file, model_data)