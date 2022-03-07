/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "Aurum.h"

using namespace Aurum;

AurumXML::AurumXML(std::shared_ptr<AccessibleNode> root) : mRoot(root)
{
    mDoc = new xml_document();
    if (mRoot) this->createXMLtree();
}

AurumXML::~AurumXML()
{
    delete mDoc;
}

void AurumXML::traverse(xml_node element, std::shared_ptr<AccessibleNode> node)
{
    if (!node) return;

    node->refresh(false);

    std::string name;
    if (node->getType().empty())
        name = node->getRole();
    else
        name = node->getType();

    // Remove white spaces.
    name.erase(remove(name.begin(), name.end(), ' '), name.end());

    element.set_name(name.c_str());

    element.append_attribute("name") = node->getText().c_str();
    element.append_attribute("type") = node->getType().c_str();
    element.append_attribute("id") = node->getId().c_str();
    element.append_attribute("automationid") = node->getAutomationId().c_str();
    element.append_attribute("package") = node->getPkg().c_str();

    const Rect<int> &size = node->getScreenBoundingBox();
    element.append_attribute("x") = size.mTopLeft.x;
    element.append_attribute("y") = size.mTopLeft.y;
    element.append_attribute("width") = size.width();
    element.append_attribute("height") = size.height();

    element.append_attribute("checked") = node->isChecked();
    element.append_attribute("checkable") = node->isCheckable();
    element.append_attribute("clickable") = node->isClickable();
    element.append_attribute("enabled") = node->isEnabled();
    element.append_attribute("focused") = node->isFocused();
    element.append_attribute("focusable") = node->isFocusable();
    element.append_attribute("scrollable") = node->isScrollable();
    element.append_attribute("selected") = node->isSelected();
    element.append_attribute("showing") = node->isShowing();
    element.append_attribute("active") = node->isActive();
    element.append_attribute("visible") = node->isVisible();
    element.append_attribute("selectable") = node->isSelectable();

    int childCnt = node->getChildCount();
    for (int i = 0; i < childCnt; i++) {
        std::shared_ptr<AccessibleNode> childNode = node->getChildAt(i);
        if (childNode == nullptr) continue;

        xml_node childElement = element.append_child("");
        traverse(childElement, childNode);
    }
}

bool AurumXML::createXMLtree()
{
    if (!mDoc || !mRoot) return false;

    mDoc->remove_children();

    xml_node element = mDoc->append_child("");

    traverse(element, mRoot);

    return true;
}

std::string AurumXML::getOptimalXPath(xml_document *doc, xml_node node)
{
    std::string xpath;

    int count = 0;
    int index = 0;

    const char *automationId = node.attribute("automationid").value();
    if (strlen(automationId) > 0) {
        xpath += "//";
        xpath += node.name();
        xpath += "[@automationid=\"";
        xpath += automationId;
        xpath += "\"]";

        try {
            auto nodes = doc->select_nodes(xpath.c_str());
            if (nodes.size() > 1) {
                for (xpath_node_set::const_iterator it = nodes.begin();
                     it != nodes.end(); ++it) {
                    xpath_node no = *it;
                    count++;
                    if (no.node() == node) {
                        index = count;
                        break;
                    }
                }
                xpath.append(1, '[')
                    .append(std::to_string(index))
                    .append(1, ']');
            }
        } catch (const xpath_exception &e) {
            LOGI("getOptimalXPath Error: %s", e.what());
        }

        return xpath;
    }

    xml_node parent = node.parent();
    if (parent) {
        xml_node child = parent.first_child();
        for (xml_node el = child; el; el = el.next_sibling()) {
            if (el.name() && node.name() && !strcmp(el.name(), node.name())) {
                count++;
                if (el == node) index = count;
            }
        }
    }

    xpath += "/";
    xpath += node.name();

    if (count > 1)
        xpath.append(1, '[').append(std::to_string(index)).append(1, ']');

    if (node == doc->first_child()) return xpath;

    return getOptimalXPath(doc, parent).append(xpath);
}

std::string AurumXML::getXPath(std::string id)
{
    if (mXPathMap.count(id) > 0) return mXPathMap[id];

    std::string query;
    query += "//*[@id=\"";
    query += id;
    query += "\"]";

    try {
        auto node = mDoc->select_node(query.c_str()).node();

        if (!node) createXMLtree();

        if (node) {
            std::string xpath = getOptimalXPath(mDoc, node);
            mXPathMap[id] = xpath;
            return xpath;
        }

    } catch (const xpath_exception &e) {
        LOGI("getXPath Error: %s", e.what());
    }

    return "NotSupported";
}