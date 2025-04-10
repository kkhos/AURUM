/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "Commands/KillServerCommand.h"
#include "Commands/FindElementCommand.h"
#include "Commands/FindElementsCommand.h"
#include "Commands/GetAttributeCommand.h"
#include "Commands/GetValueCommand.h"
#include "Commands/SetValueCommand.h"
#include "Commands/ClearCommand.h"
#include "Commands/GetSizeCommand.h"
#include "Commands/ClickCommand.h"
#include "Commands/FlickCommand.h"
#include "Commands/LongClickCommand.h"
#include "Commands/TouchDownCommand.h"
#include "Commands/TouchMoveCommand.h"
#include "Commands/TouchUpCommand.h"
#include "Commands/CloseAppCommand.h"
#include "Commands/GetAppInfoCommand.h"
#include "Commands/InstallAppCommand.h"
#include "Commands/LaunchAppCommand.h"
#include "Commands/RemoveAppCommand.h"
#include "Commands/GetDeviceTimeCommand.h"
#include "Commands/GetLocationCommand.h"
#include "Commands/SendKeyCommand.h"
#include "Commands/TakeScreenshotCommand.h"
#include "Commands/DumpObjectTreeCommand.h"
#include "Commands/GetScreenSizeCommand.h"
#include "Commands/ActionAndWaitEventCommand.h"
#include "Commands/SetFocusCommand.h"
#include "Commands/MoveToCommand.h"
#include "Commands/GetActiveAppToolkitNameCommand.h"
#include "Commands/GetTextMinBoundingRectCommand.h"
#include "Commands/SetXMLSyncCommand.h"
#include "Commands/GetAngleCommand.h"
#include "Commands/NextCommand.h"
#include "Commands/PrevCommand.h"
#include "Commands/FirstCommand.h"
#include "Commands/LastCommand.h"
#include "Commands/GetParentCommand.h"
#include "Commands/MouseDownCommand.h"
#include "Commands/MouseMoveCommand.h"
#include "Commands/MouseUpCommand.h"
#include "Commands/SetIncludeHiddenCommand.h"
#include "Commands/GetIncludeHiddenCommand.h"
#include "Commands/AddWatcherCommand.h"
#include "Commands/ClearWatcherCommand.h"
