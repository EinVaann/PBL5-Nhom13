from django.contrib import admin
from .models import Speech, User

admin.site.site_header = "Speech Admin"
admin.site.site_title = "Speech Admin Area"
admin.site.index_title = "Welcome to Speech Admin"
admin.site.register(Speech)
# admin.site.register(User)
