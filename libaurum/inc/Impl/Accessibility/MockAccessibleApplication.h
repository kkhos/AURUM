/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef _MOCK_ACCESSIBLE_APPLICATION_H_
#define _MOCK_ACCESSIBLE_APPLICATION_H_

#include "AccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

#include <memory>

using namespace Aurum;

namespace AurumInternal {

namespace Mock {

class MockAccessibleApplication : public AccessibleApplication, public std::enable_shared_from_this<MockAccessibleApplication> {
public:
    /**
     * @brief TBD
     * @since tizen_6.0
     */
    MockAccessibleApplication(std::shared_ptr<AccessibleNode> node);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    MockAccessibleApplication(std::shared_ptr<AccessibleNode> parent, std::string text,std::string pkg,std::string role, std::string res,std::string type,std::string style,Rect<int> screenBoundingBox,int supportingIfaces,int featureProperty);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    virtual ~MockAccessibleApplication();

public:
    /**
     * @brief TBD
     * @since tizen_6.0
     */
    std::vector<std::shared_ptr<AccessibleWindow>> getWindows(void) override;

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    std::string getPackageName(void) override;

public:
    /**
     * @brief TBD
     * @since tizen_6.0
     */
    void addWindow(std::shared_ptr<AccessibleWindow> window);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    std::shared_ptr<MockAccessibleWindow> addWindow(std::string title, std::string widget, Rect<int> geometry, int properties);

    /**
     * @brief TBD
     * @since tizen_6.0
     */
    void clearWindows(void);

private:
    /**
     * @brief TBD
     */
    std::vector<std::shared_ptr<AccessibleWindow>> mWindowList;
};

}

}

#endif
