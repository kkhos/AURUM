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
#ifndef _AU_OBJECT_H_
#define _AU_OBJECT_H_

namespace aurum
{
    class AuObject
    {
        public:
            AuObject();
            ~AuObject();
            //FIXME: hide this
            void setName(std::string objectName);
            std::string getName();
            std::string getText();
            void setText(std::string text);
            std::string getType();
            AuObject* getParent();
            std::vector<AuObject *> getChildren();
            AuObject* findObject();
            std::vector<AuObject *> findObjects();
            Rect<int> getGeometry();
            bool isClickable();
            bool isFocusable();
            bool isFocused();
            bool isShowing();
            bool isVisible();
            bool click();
            void longClick(const unsigned int intv = LOGNCLICK_INTERVAL);

        private:
            std::string mName;
            static const unsigned int LOGNCLICK_INTERVAL = 50;
    };

}  // namespace aurum

#endif /* _AU_OBJECT_H_ */
