from django import forms
from .models import Device

class device_form(forms.ModelForm):
    required_css_class = 'required-field'
    name = forms.CharField()
    light = forms.BooleanField(widget=forms.CheckboxInput(attrs={'id':'device_light','onChange':"this.form.submit()"}), required=False)
    # ,'onchange':"alert('this')"
    fan = forms.BooleanField(widget=forms.CheckboxInput(attrs={'id':'device_fan','onChange':"this.form.submit()"}), required=False)
    music = forms.BooleanField(widget=forms.CheckboxInput(attrs={'id':'device_music','onChange':"this.form.submit()"}), required=False)
    class Meta:
        model = Device
        fields = ['name','light','fan','music']