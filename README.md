# AURUM(Tizen UI Automator)
- Aurum is a UI automation framework without UI Toolkit dependency.

  Provides Commands to interact with the device¡¯s UI by simulation user actions and introspection of the screen content.

  It relies on the platform accessibility APIs to introspect the screen.

- User can use the IDL defined in Proto to create an automation app or script in a variety of languages without a language dependency.

  [List of Supported Command With Python Example](https://code.sec.samsung.net/confluence/pages/viewpage.action?pageId=212993496)


### Running on TV
- Pre-condition

  The TV device and Host PC should be already connected through SDB

- Gets Aurum latest version and checkout to tizen branch

  [Aurum Github](https://github.sec.samsung.net/tizen/aurum)

- Set up a python virtual environment (working directory: aurum/)

  (host) cd ui_automation/python/tv

  #### Create virtual env
  (host) python3 -m venv v

  #### Activate a virtual env
  Linux
  (host) source v/bin/activate
  Window
  (host) v/Scripts/activate.bat

  #### Install required pkg (only once)
  (python_virtual) pip3 install -r ../../../protocol/resources/python/requirements.txt

  #### Generate aurum.proto file for python (only once)
  (python_virtual) python3 -m grpc_tools.protoc --python_out=./ --grpc_python_out=./ -I ./../../../protocol/ ../../../protocol/aurum.proto

  #### Target setup such as sdb forward, bootstrap execution
  (python_virtual) python3 ../../../protocol/resources/python/tv/tvSetup.py

  #### Then Run your test script (Please refer sample scripts in 'aurum/protocol/resources/python/tv')
  (python_virtual) python3 myTest.py

  #### Deactivate a virtual env
  (python_virtual) deactivate

### Reference and Tip
- [How to run aurum on TM1](https://code.sec.samsung.net/confluence/display/GFX/04.+NUITizenGallery+Test+Script+Guide)

