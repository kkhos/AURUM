# AURUM(Tizen UI Automator)
- Aurum is a UI automation framework without UI Toolkit dependency.

  Provides Commands to interact with the device¡¯s UI by simulation user actions and introspection of the screen content.

  It relies on the platform accessibility APIs to introspect the screen.

- User can use the IDL defined in Proto to create an automation app or script in a variety of languages without a language dependency.

  [List of Supported Command With Python Example](https://code.sec.samsung.net/confluence/pages/viewpage.action?pageId=212993496)


### Running on TV
- Gets Aurum latest version and checkout to tizen branch

  [Aurum Github](https://github.sec.samsung.net/tizen/aurum)

- Set up a python virtual environment (working directory: aurum/)

   (host) cd ui_automation/python/tv

   (host) python3 -m venv v   (create virtual env)

   (host) linux: source v/bin/activate

   (python_virtual) pip3 install -r ../../../protocol/resources/python/requirements.txt   (only once)

   (python_virtual) python3 -m grpc_tools.protoc --python_out=./ --grpc_python_out=./ -I ./../../../protocol/ ../../../protocol/aurum.proto   (only once)

   (python_virtual) python3 ../../../protocol/resources/python/tv/tvSetup.py

   Then Run your script
   ex: python3 NUITizenGallery.py


### Reference and Tip
- [How to run aurum on TM1](https://code.sec.samsung.net/confluence/display/GFX/04.+NUITizenGallery+Test+Script+Guide)

