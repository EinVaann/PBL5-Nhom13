from django.contrib import admin
from .models import Audiofile

# Register your models here.
class dataAdmin(admin.ModelAdmin):
    list_display = ['id','name','audio']
admin.site.register(Audiofile,dataAdmin)