# test python script must be in same location as aurum_pb2.py

# python3 -m venv v
# source v/bin/activate
# pip3 install grpcio grpcio-tools
#
# python3 -m grpc_tools.protoc --python_out=. --grpc_python_out=. --proto_path=. aurum.proto
#
# python3 <file>.py
# deactivate 

from utils import *

#4 Menu Control Test
def ST04_MENU_CONTROL():
    print("- Menu Test")
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86SysMenu", "org.tizen.menu", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_CONTENT_LOAD) == False:
        return False 
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_AFTER_KEY)
    TAKE_SCREENSHOT(ST04_MENU_CONTROL.__name__ + '_Menu.png')
    SEND_KEY("XF86Back", WAIT_AFTER_KEY)
    SEND_KEY("XF86Back", WAIT_AFTER_KEY)
    SEND_KEY("XF86Back", WAIT_AFTER_KEY)

    HOME()

    print("- Broadcasting Info Test")
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86ChannelGuide", "org.tizen.epg", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        return False 
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    TAKE_SCREENSHOT(ST04_MENU_CONTROL.__name__ + '_Broadcasting_Info.png')

    HOME()

    print("- Channel Info Test")
    SEND_KEY_WAIT_EVENT("Return", "EVENT_WINDOW_ACTIVATE", WAIT_FOR_APP_TRANSFROM)
    #Wait for automatically activated channel info disappear
    time.sleep(WAIT_FOR_APP_CONTENT_LOAD)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.hbbtv", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    TAKE_SCREENSHOT(ST04_MENU_CONTROL.__name__ + '_Channel_Info.png')

    return True

#8 My content Test
def ST08_MY_CONTENT():
    print("- My Contents Launching")
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.mycontents", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    TAKE_SCREENSHOT(ST08_MY_CONTENT.__name__ + '_Launched.png')

    print("- My Contents Video Play")
    SEND_KEY("Return", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.mycontent-video-player-tv", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_CONTENT_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    SEND_KEY("Right", WAIT_AFTER_KEY)    
    SEND_KEY("Left", WAIT_AFTER_KEY)
    TAKE_SCREENSHOT(ST08_MY_CONTENT.__name__ + '_Video_Player.png')
    SEND_KEY("XF86Back", WAIT_AFTER_KEY)    
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86Back", "com.samsung.tv.mycontents", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_CONTENT_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    
    print("- My Contents Photo Play")
    SEND_KEY("Down", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.mycontent-photo-player-tv", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_CONTENT_LOAD) == False:
        RESET_TO_TVPLUS()
        return False        
    TAKE_SCREENSHOT(ST08_MY_CONTENT.__name__ + '_Photo_Player.png')
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86Back", "com.samsung.tv.mycontents", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_CONTENT_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    
    print("- My Contents Music Play")
    SEND_KEY("Left", WAIT_AFTER_KEY)    
    SEND_KEY_WAIT_EVENT("Return", "EVENT_WINDOW_IDLE", WAIT_FOR_APP_CONTENT_LOAD) #Wait till view transformed
    TAKE_SCREENSHOT(ST08_MY_CONTENT.__name__ + '_Music_Play.png')

    #Back to default view
    SEND_KEY("XF86Back", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)


    return True

#7, 9, 16, 17. App Download, install and run
def ST07_09_16_17_APP_INSTALL():
    print("- Apps Launching")
    #FIXME: Use XPath to find APPS
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Left", WAIT_AFTER_KEY)
    
    print("- Search TVING Application")
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.store", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY_WAIT_EVENT("Return", "EVENT_WINDOW_IDLE", WAIT_FOR_APP_TRANSFROM) #Wait till view transformed
    #FIXME: Input text directly or other way :(
    #FIXME: If keypboard already in English mode. it will fail :(
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM) #Change Lang
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Left", WAIT_AFTER_KEY) 
    SEND_KEY("Return", WAIT_AFTER_KEY) #t
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)    
    SEND_KEY("Return", WAIT_AFTER_KEY) #v
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_AFTER_KEY) #i
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY_WAIT_EVENT("Return", "EVENT_WINDOW_IDLE", WAIT_AFTER_KEY) #Select TVING App
    #FIXME: TVing isInstalled is not working
    print("- Install TVing")
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.notification-app", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False
    TAKE_SCREENSHOT(ST07_09_16_17_APP_INSTALL.__name__ + '_Install_Notification.png')
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86Exit", "com.samsung.tv.store", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False
    print("- Launch TVing")
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", TVING_APP, WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        return False
    TAKE_SCREENSHOT(ST07_09_16_17_APP_INSTALL.__name__ + '_TVing_Launch.png')
    print("- Delete TVing")
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86Exit", "com.samsung.tv.csfs", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_CONTENT_LOAD) == False:
        return False 
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.store", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_CONTENT_LOAD) == False:
        return False
    SEND_KEY("XF86Back", WAIT_FOR_APP_TRANSFROM)
    SEND_KEY("XF86Back", WAIT_FOR_APP_TRANSFROM)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)
    SEND_KEY("Down", WAIT_AFTER_KEY) 
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)
    SEND_KEY("Left", WAIT_AFTER_KEY) 
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)
    TAKE_SCREENSHOT(ST07_09_16_17_APP_INSTALL.__name__ + '_TVing_Delete.png')
    
    #FIXME: Check app is not installed
    return True

def ST10_11_12_CHANNEL_VOLUME_CHANGE():
    #Csfs -> DTV or TV Plus (tv-viewer)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.tv-viewer", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False

    #Volume Changes
    SEND_KEY_WAIT_EVENT("XF86AudioRaiseVolume", "EVENT_WINDOW_RAISE", WAIT_AFTER_KEY)
    StubSingleton.get_stub().sendKey(ReqKey(type="XF86", actionType="REPEAT", XF86keyCode="XF86AudioRaiseVolume", durationMs=2000, intervalMs=500))
    TAKE_SCREENSHOT(ST10_11_12_CHANNEL_VOLUME_CHANGE.__name__ + '_Volume_Up.png')

    SEND_KEY_WAIT_EVENT("XF86AudioLowerVolume", "EVENT_WINDOW_RAISE", WAIT_AFTER_KEY)
    StubSingleton.get_stub().sendKey(ReqKey(type="XF86", actionType="REPEAT", XF86keyCode="XF86AudioLowerVolume", durationMs=2000, intervalMs=500))
    TAKE_SCREENSHOT(ST10_11_12_CHANNEL_VOLUME_CHANGE.__name__ + '_Volume_Down.png')

    if SEND_KEY_WAIT_APP_LAUNCHING("XF86Home", "com.samsung.tv.csfs", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False

    #Csfs -> DTV or TV Plus (tv-viewer)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.tv-viewer", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False

    #Channel Changes
    SEND_KEY_WAIT_EVENT("XF86RaiseChannel", "EVENT_WINDOW_RAISE", WAIT_AFTER_KEY)
    StubSingleton.get_stub().sendKey(ReqKey(type="XF86", actionType="REPEAT", XF86keyCode="XF86RaiseChannel", durationMs=2000, intervalMs=500))
    TAKE_SCREENSHOT(ST10_11_12_CHANNEL_VOLUME_CHANGE.__name__ + '_Channel_Up.png')

    SEND_KEY_WAIT_EVENT("XF86LowerChannel", "EVENT_WINDOW_RAISE", WAIT_AFTER_KEY)
    StubSingleton.get_stub().sendKey(ReqKey(type="XF86", actionType="REPEAT", XF86keyCode="XF86LowerChannel", durationMs=2000, intervalMs=500))
    TAKE_SCREENSHOT(ST10_11_12_CHANNEL_VOLUME_CHANGE.__name__ + '_Channel_Down.png')

    return True

def ST13_BROWSER():
    CHECK_APP_INSTALLED()
    print("- Launch browser")
    SEND_KEY("Up", WAIT_AFTER_KEY)   
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.browser", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    TAKE_SCREENSHOT(ST13_BROWSER.__name__ + '_Launched.png')

    print("- TV On/OFF Test in Browser")
    SEND_KEY("XF86PowerOff", WAIT_AFTER_KEY)
    time.sleep(10)
    SEND_KEY("XF86PowerOff", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("", "org.tizen.browser", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD):
        return False
    CHECK_NOTI_POPUP()
    TAKE_SCREENSHOT(ST13_BROWSER.__name__ + '_After_Turn_Off.png')

    RESET_TO_TVPLUS()

    return True

def ST14_15_YOUTUBE():
    CHECK_APP_INSTALLED()
    print("- Launch youtube")
    SEND_KEY("Up", WAIT_AFTER_KEY)   
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.cobalt-yt", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    TAKE_SCREENSHOT(ST14_15_YOUTUBE.__name__ + '_Launched.png')

    print("- TV On/OFF Test in Youtube")
    SEND_KEY("XF86PowerOff", WAIT_AFTER_KEY)
    time.sleep(10)
    SEND_KEY("XF86PowerOff", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("", "com.samsung.tv.cobalt-yt", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD):
        return False
    CHECK_NOTI_POPUP()
    TAKE_SCREENSHOT(ST14_15_YOUTUBE.__name__ + '_After_Turn_Off.png')

    RESET_TO_TVPLUS()

    return True

def ST18_MULTITASKING():
    print("- Launch youtube")
    SEND_KEY("Up", WAIT_AFTER_KEY)   
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.cobalt-yt", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        RESET_TO_TVPLUS()
        return False
    TAKE_SCREENSHOT(ST18_MULTITASKING.__name__ + '_Youtube_Launched.png')
    SEND_KEY("Return", WAIT_AFTER_KEY)
    time.sleep(10) #Play video

    print("- Launch csfs")
    if SEND_KEY_WAIT_APP_LAUNCHING("XF86Home", "com.samsung.tv.csfs", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_TRANSFROM) == False:
        return False
    TAKE_SCREENSHOT(ST18_MULTITASKING.__name__ + '_csfs_Launched.png')

    print("- Launch browser")
    SEND_KEY("Up", WAIT_AFTER_KEY)   
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.browser", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        for i in range(6):
            SEND_KEY("Left", WAIT_AFTER_KEY)
        SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)
        return False
    TAKE_SCREENSHOT(ST18_MULTITASKING.__name__ + '_browser_Launched.png')
    RESET_TO_TVPLUS()

    return True

def ST19_20_NETFLIX():
    CHECK_APP_INSTALLED()
    print("- Launch netflix")
    SEND_KEY("Up", WAIT_AFTER_KEY)   
    SEND_KEY("Right", WAIT_AFTER_KEY)
    #nexflix 
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.netflix-app", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        RESET_TO_TVPLUS() 
        return False
    TAKE_SCREENSHOT(ST19_20_NETFLIX.__name__ + '_Netflix_Launched.png')

    print("- TV On/OFF Test in Netflix")
    SEND_KEY("XF86PowerOff", WAIT_AFTER_KEY)
    time.sleep(10)
    SEND_KEY("XF86PowerOff", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("", "org.tizen.netflix-app", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD):
        return False
    CHECK_NOTI_POPUP()
    TAKE_SCREENSHOT(ST19_20_NETFLIX.__name__ + '_After_Turn_Off.png')

    RESET_TO_TVPLUS()

    return True

def ST21_22_AMBIENT():
    print("- Run Ambient")
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY_WAIT_EVENT("Return", "EVENT_WINDOW_IDLE", WAIT_AFTER_KEY)
    TAKE_SCREENSHOT(ST21_22_AMBIENT.__name__ + '_Ambient_Start.png')

    print("- Launch Art-app")
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY) 
    SEND_KEY("Down", WAIT_AFTER_KEY) 
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "org.tizen.art-app", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LAUNCH) == False:
        return False
    TAKE_SCREENSHOT(ST21_22_AMBIENT.__name__ + '_Artapp_Launched.png')

    #FIXME: We need to test ST_22 but embient key is not exist.
    #Back to csfs home view
    SEND_KEY("XF86Exit", WAIT_FOR_APP_LOAD)
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY_WAIT_EVENT("Return", "EVENT_WINDOW_IDLE", WAIT_AFTER_KEY)

    return True

def ST23_MULTIVIEW():
    SEND_KEY("Left", WAIT_AFTER_KEY)    
    SEND_KEY("Down", WAIT_AFTER_KEY) 
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)
    SEND_KEY("Right", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.multiscreen", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        return False
    TAKE_SCREENSHOT(ST23_MULTIVIEW.__name__ + '_Multiview_Launched.png')    

    RESET_TO_TVPLUS()

    return True

def ST24_ROTATION():
    print("- Device rotation to 90, 180, 270, 0")
    degrees = [90, 180, 270, 0]

    for degree in degrees:
        DEVICE_ROTATION(degree)
        time.sleep(WAIT_FOR_APP_TRANSFROM)
        TAKE_SCREENSHOT(ST24_ROTATION.__name__ + '_' + str(degree) + '.png')

    return True

def ST25_SEARCH():
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Up", WAIT_AFTER_KEY)
    if SEND_KEY_WAIT_APP_LAUNCHING("Return", "com.samsung.tv.searchall", WAIT_FOR_APP_LAUNCH, WAIT_FOR_APP_LOAD) == False:
        return False
    TAKE_SCREENSHOT(ST25_SEARCH.__name__ + '_Searchall_Launched.png')
    SEND_KEY("Up", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM) #a
    SEND_KEY("Right", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_FOR_APP_CONTENT_LOAD) #b    
    TAKE_SCREENSHOT(ST25_SEARCH.__name__ + '_Searchall_Searching.png')

    #Return to Korean Keyboard
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Return", WAIT_FOR_APP_TRANSFROM)

    return True

def ST26_CSFS():
    print("- CSFS test")
    SEND_KEY("Up", WAIT_AFTER_KEY)
    StubSingleton.get_stub().sendKey(ReqKey(type="XF86", actionType="REPEAT", XF86keyCode="Right", durationMs=5000, intervalMs=50)) # long press key
    TAKE_SCREENSHOT(ST26_CSFS.__name__ + '_LongPress_Move.png')
    
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    TAKE_SCREENSHOT(ST26_CSFS.__name__ + '_Thumbnail_Move.png')

    SEND_KEY("Left", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    SEND_KEY("Down", WAIT_AFTER_KEY)
    StubSingleton.get_stub().sendKey(ReqKey(type="XF86", actionType="REPEAT", XF86keyCode="Right", durationMs=5000, intervalMs=50)) # long press key
    TAKE_SCREENSHOT(ST26_CSFS.__name__ + '_Setting_Move.png')

    return True

def PRE_TEST():
    CHECK_NOTI_POPUP()
    HOME()

def POST_TEST():
    HOME()

def runTest(testFunc):
    global SUCCESS
    global FAIL
    global TEST_RESULT
    print("Test Start : ", testFunc)
    PRE_TEST()
    ret = testFunc()
    POST_TEST()
    print("Test Finish Result = " + str(ret) + "\n")
    if ret:
        SUCCESS += 1
    else:
        FAIL += 1

    TEST_RESULT += [
        {
            'test_name'  : str(testFunc).split(' ')[1],
            'test_result': str(ret)
        }
    ]

def run():
    startTime = time.time()
    
    INIT_VD_SMOKE_TEST()

    #Start TV Smoke Test
    if CHECK_USB_PLUGGED():
        runTest(ST08_MY_CONTENT)
    runTest(ST10_11_12_CHANNEL_VOLUME_CHANGE)
    runTest(ST26_CSFS)
    runTest(ST24_ROTATION)
    runTest(ST13_BROWSER)
    runTest(ST19_20_NETFLIX)
    runTest(ST14_15_YOUTUBE)
    runTest(ST18_MULTITASKING)
    runTest(ST23_MULTIVIEW)
    runTest(ST21_22_AMBIENT)
    runTest(ST04_MENU_CONTROL)
    runTest(ST07_09_16_17_APP_INSTALL)
    runTest(ST25_SEARCH)

    CREATE_HTML_FILE()

    print("--- Total Cases : {} [ Success : {} Fail : {} ] ---\n".format(SUCCESS+FAIL, SUCCESS, FAIL))
    print("--- Test takes " + f"{time.time() - startTime: .3f} sec ---\n")

    print("--- Exist CS file list ---")
    CHECK_CRASH_REPORT()
 
if __name__ == '__main__':                                         
    run()
