from __future__ import print_function
import os
import subprocess
import re
import sys
import time
import datetime
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import grpc
import numpy as np
import cv2
from PIL import Image

#Defined values
WAIT_AFTER_KEY = 1
WAIT_FOR_APP_TRANSFROM = 3
WAIT_FOR_APP_CONTENT_LOAD = 5
WAIT_FOR_APP_LOAD = 10
WAIT_FOR_APP_LAUNCH = 30
WAIT_FOR_CONTENT_DOWNLOAD = 40
TIMEOUT_MS = 5000
LOGIN = 0
TVING_APP = "HLoFAbhj6Q.TVING"
SUCCESS = 0
FAIL = 0

global DEVICE_INFO
global FOLDER_NAME
TEST_RESULT = []

#Class for Stub singleton
class StubSingleton:
    _instance = None

    @classmethod
    def get_stub(cls):
        if cls._instance is None:
            cls._instance = BootstrapStub(grpc.insecure_channel('127.0.0.1:50051', options=[ ('grpc.max_send_message_length', 1920*1080*4+5), ('grpc.max_receive_message_length', 1920*1080*4+5)]))
        return cls._instance

#Check given applicaiton running state
def CHECK_APP_RUNNING(app):
    return StubSingleton.get_stub().getAppInfo(ReqGetAppInfo(packageName=app)).isFocused

#Please given message and back to home
def IS_APP_LAUNCHING_FAIL(app):
    if CHECK_APP_RUNNING(app) == False:
        print(app +" launching fail Back to home")
        SEND_KEY_WAIT_EVENT("XF86Home", "EVENT_WINDOW_ACTIVATE", WAIT_FOR_APP_TRANSFROM)
        return False
    return True

#Send key and wait event untill waitSec then sleep for delay time
def SEND_KEY_WAIT_APP_LAUNCHING(key, app, waitSec, delay):
    launch_request = time.time()
    SEND_KEY(key, WAIT_AFTER_KEY)
    while 1:
        time.sleep(0.1)
        if CHECK_APP_RUNNING(app) == True:
            time.sleep(delay)
            print(app + " Launched")
            break
        if time.time() - launch_request > waitSec:
            print(app + " Launch Fail")
            return False

#Take screenshot with given file name
def TAKE_SCREENSHOT(name):
    global FOLDER_NAME
    responses = StubSingleton.get_stub().takeScreenshot(ReqTakeScreenshot(getPixels=True))
    for response in responses:
        buf = np.frombuffer(response.image, dtype=np.uint8).reshape(1080,1920,4)
        image = cv2.cvtColor(buf, cv2.COLOR_RGBA2RGB)
        save_location = f"{FOLDER_NAME}/{name}"
        cv2.imwrite(save_location, image)
        #with open(name, 'wb') as fout:
        #    cv2.imwrite(name, image)

#Run command in device shell
def RUN_COMMAND(command):
    stream = os.popen(command)
    output = stream.read()
    return output

#Login samsung account
def SAMSUNG_ACCOUNT_LOGIN():
    global LOGIN
    if LOGIN:
        print("Already setup samsung account\n")
    else:
        RUN_COMMAND("sdb shell ssoscmd -c 4 -d iottest0909@gmail.com@SSOSVCAPI@1q2w3e")
        print("Samsung account login\n")
        LOGIN = 1
        time.sleep(10)

#Device Rotation with given degree
def DEVICE_ROTATION(degree):
    RUN_COMMAND("sdb shell winfo -rotation set " + str(degree))

#Send given key event and wait given specific event till @TIMEOUT_MS then wait some time
"""
      EVENT_WINDOW_ACTIVATE
      EVENT_WINDOW_DEACTIVATE
      EVENT_WINDOW_MINIMIZE
      EVENT_WINDOW_RAISE
      EVENT_WINDOW_IDLE
      EVENT_STATE_CHANGED_FOCUSED
"""
def SEND_KEY_WAIT_EVENT(key, event, delay):
    StubSingleton.get_stub().actionAndWaitEvent(ReqActionAndWaitEvent(type="KEY", XF86keyCode=key, eventType=event, timeoutMs=TIMEOUT_MS))
    time.sleep(delay)

#Send key event and wait some time
def SEND_KEY(key, delay):
    StubSingleton.get_stub().sendKey(ReqKey(type="XF86", actionType="STROKE", XF86keyCode=key))
    time.sleep(delay)

#Back to home
def HOME():
    SEND_KEY_WAIT_EVENT("XF86Home", "EVENT_WINDOW_ACTIVATE", WAIT_FOR_APP_TRANSFROM)

#Check notification popup activated or not it also can test "7 Global Notification Test"
def CHECK_NOTI_POPUP():
    if StubSingleton.get_stub().getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.notification-app')).isRunning:
        TAKE_SCREENSHOT('ST7_NOTI_POPUP.png')
        print("#7. Global Notification")
        SEND_KEY_WAIT_EVENT("Return", "EVENT_WINDOW_DEACTIVATE")
        time.sleep(WAIT_FOR_APP_TRANSFROM)

#Check sdb connected or not
def CHECK_SDB():
    stream = os.popen("command -v sdb")
    output = stream.read()
    if len(output) > 0:
        return True
    else:
        print("Error - Can't run sdb")
        return False

#Reset to TVPlus
def RESET_TO_TVPLUS():
    print("Reset to TVPlus")
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86Home", "com.samsung.tv.csfs", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        return False
    SEND_KEY("Up", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.tv-viewer", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False

#Print testing device env
def GET_DEVICE_INFO():
    global DEVICE_INFO
    print("--- Test Device Info ---")
    stream = os.popen("sdb shell cat /etc/info.ini")
    output = stream.read()
    DEVICE_INFO = output
    print(DEVICE_INFO)

#Check aurum ready or not
def CHECK_AURUM_READY():
    stream = os.popen("sdb shell 'ps -e | grep aurum-bootstrap'")
    output = stream.read()
    if len(output) > 0:
        return True
    else:
        print("Error - aurum-bootstrap not launched on target")
        print("Try to launch aurum-bootstrap")
        stream = os.popen("sdb shell app_launcher -s org.tizen.aurum-bootstrap")
        output = stream.read()
        time.sleep(1)
        if len(output) > 0:
            print("Success to launch aurum-bootstrap")
            return True

        print("Error - Can't launch aurum-bootstrap")
        return False

#Create folder to save imgs
def CREATE_FOLDER_TO_SAVE_CAPTURES():
    now = datetime.datetime.now()
    global FOLDER_NAME
    FOLDER_NAME = now.strftime("VD_ST_%Y-%m-%d_%H-%M-%S")
    os.mkdir(FOLDER_NAME)

def CHECK_APP_INSTALLED():
    print("--- Check needed app installed")

    while True:
        netflix = StubSingleton.get_stub().getAppInfo(ReqGetAppInfo(packageName='org.tizen.netflix-app')).isInstalled
        youtube = StubSingleton.get_stub().getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.cobalt')).isInstalled
        browser = StubSingleton.get_stub().getAppInfo(ReqGetAppInfo(packageName='org.tizen.browser')).isInstalled
        if netflix and youtube and browser:
            break
        print("Waiting app installed [ netflix: {} , youtube: {} , browser: {} ] ".format(netflix, youtube, browser))
        time.sleep(5)

    HOME()

def CHECK_CRASH_REPORT():
    print(RUN_COMMAND("sdb shell ls /opt/share/crash/report"))

def CHECK_ELM_ATSPI_MODE():
    output = RUN_COMMAND("sdb shell cat /etc/profile.d/efl.sh")
    target_string = "ELM_ATSPI_MODE"

    if target_string in output:
        return True
    else:
        print("Error - Need to add 'ELM_ATSPI_MODE=1' in /etc/profile.d/efl.sh then reboot please. ")
        return False

def CHECK_USB_PLUGGED():
    output = RUN_COMMAND("sdb shell lsblk")
    target_string = "USB"

    if target_string in output:
        return True
    else:
        print("NOTICE - USB is not Plugged. Test can generated except ST08_MY_CONTENT\n")
        return False

def FIND_IMAGE_WITH_PREFIX(folder_name, prefix):
    image_files = []

    for file_name in os.listdir(folder_name):
        file_path = os.path.join(folder_name, file_name)

        if file_name.startswith(prefix):
            image_files.append(file_path)

    image_files.sort()
    return image_files

def FILTER_ITEMS_WITH_STRING(arr, search_string):
    filtered_arr = [item for item in arr if search_string in item]
    return filtered_arr

def CREATE_HTML_FILE():
    sorted_result = sorted(TEST_RESULT, key=lambda x: x['test_name'])
    expected_imgs = GET_EXPECTED_IMAGE_LIST()

    html_content = f'''
    <!DOCTYPE html>
    <html>
    <head>
      <title>Test Results</title>
      <div>Test device info : <br> {DEVICE_INFO}</div>
      <style>
        table {{
          border-collapse: collapse;
          width: 100%;
        }}
        th, td {{
          border: 1px solid black;
          padding: 8px;
          text-align: left;
        }}
      </style>
    </head>
    <body>
      <h1>Test Results</h1>
      <table>
        <tr>
          <th>Test Name</th>
          <th>Test Result</th>
          <th>Expected Image - Tizen 7.0 230516</th>
          <th>Test Image</th>
        </tr>
    '''

    for test in sorted_result:
        test_name = test['test_name']
        test_result = test['test_result']
        test_expected_imgs = FILTER_ITEMS_WITH_STRING(expected_imgs, test_name)
        test_images = FIND_IMAGE_WITH_PREFIX(FOLDER_NAME, test_name)

        html_content += f'''
        <tr>
          <td rowspan={len(test_expected_imgs)}>{test_name}</td>
          <td rowspan={len(test_expected_imgs)}>{test_result}</td>
        '''

        for idx in range(len(test_expected_imgs)):
            if idx != 0:
                html_content += f'''
                <tr>
                '''

            html_content += f'''
                <td><img src="{test_expected_imgs[idx]}" width="480" height="270" alt="{test_name} Image"></td>
            '''

            if idx < len(test_images):
                html_content += f'''
                    <td><img src="{test_images[idx]}" width="480" height="270" alt="{test_name} Image"></td>
                '''

            html_content += f'''
            </tr>
            '''

    html_content += '''
      </table>
    </body>
    </html>
    '''

    result = f"{FOLDER_NAME}_Test_Result.html"
    with open(result, 'w') as file:
        file.write(html_content)

def GET_EXPECTED_IMAGE_LIST():
    img_list = [
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST04_MENU_CONTROL_Broadcasting_Info.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST04_MENU_CONTROL_Channel_Info.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST04_MENU_CONTROL_Menu.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST07_09_16_17_APP_INSTALL_Install_Notification.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST07_09_16_17_APP_INSTALL_TVing_Delete.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST07_09_16_17_APP_INSTALL_TVing_Launch.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST08_MY_CONTENT_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST08_MY_CONTENT_Music_Play.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST08_MY_CONTENT_Photo_Player.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST08_MY_CONTENT_Video_Player.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST10_11_12_CHANNEL_VOLUME_CHANGE_Channel_Down.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST10_11_12_CHANNEL_VOLUME_CHANGE_Channel_Up.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST10_11_12_CHANNEL_VOLUME_CHANGE_Volume_Down.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST10_11_12_CHANNEL_VOLUME_CHANGE_Volume_Up.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST13_BROWSER_After_Turn_Off.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST13_BROWSER_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST14_15_YOUTUBE_After_Turn_Off.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST14_15_YOUTUBE_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST18_MULTITASKING_Youtube_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST18_MULTITASKING_browser_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST18_MULTITASKING_csfs_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST19_20_NETFLIX_After_Turn_Off.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST19_20_NETFLIX_Netflix_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST21_22_AMBIENT_Ambient_Start.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST21_22_AMBIENT_Artapp_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST23_MULTIVIEW_Multiview_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST24_ROTATION_0.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST24_ROTATION_180.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST24_ROTATION_270.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST24_ROTATION_90.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST25_SEARCH_Searchall_Launched.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST25_SEARCH_Searchall_Searching.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST26_CSFS_LongPress_Move.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST26_CSFS_Setting_Move.png',
        'https://pages.github.sec.samsung.net/NUI/AurumTestScript/ReleaseTest/VDSmokeTest/Expected_Images/ST26_CSFS_Thumbnail_Move.png'
    ]

    return img_list

#Configure Smoke Test
def INIT_VD_SMOKE_TEST():
    # Command Timeout Setting
    StubSingleton.get_stub().setTimeout(ReqSetTimeout(timeout = TIMEOUT_MS))
    if not CHECK_ELM_ATSPI_MODE():
        sys.exit()
    if not CHECK_SDB():
        sys.exit()
    if not CHECK_AURUM_READY():
        sys.exit()
    GET_DEVICE_INFO()
    SAMSUNG_ACCOUNT_LOGIN()
    CREATE_FOLDER_TO_SAVE_CAPTURES()
