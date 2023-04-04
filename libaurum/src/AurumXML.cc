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

AurumXML::AurumXML(const std::shared_ptr<AccessibleNode> root, int *appXMLLoadedCount, std::mutex *XMLMutex, std::condition_variable *XMLConditionVar)
: mRoot(root)
{
    mDoc = new xml_document();
    if (mRoot) this->createXMLtree();

    XMLMutex->lock();
    LOGI("XML Document Created: %s", root->getId().c_str());
    (*appXMLLoadedCount)++;
    XMLMutex->unlock();
    XMLConditionVar->notify_all();
}

AurumXML::~AurumXML()
{
    delete mDoc;
}

void AurumXML::traverse(xml_node& element, const std::shared_ptr<AccessibleNode>& node)
{
    if (!node) return;

    node->updateUniqueId();
    node->updateName();
    node->updateRoleName();
    node->updateAttributes();
    node->updateToolkitName();

    std::string name;
    if (node->getType().empty())
        name = node->getRole();
    else
        name = node->getType();

    if (!name.compare("application")) name = node->getPkg();

    // Remove white spaces.
    name.erase(remove(name.begin(), name.end(), ' '), name.end());

    element.set_name(name.c_str());

    element.append_attribute("name") = node->getText().c_str();
    element.append_attribute("id") = node->getId().c_str();
    element.append_attribute("automationid") = node->getAutomationId().c_str();

    mXNodeMap[node->getId()] = node;

    auto children = node->getChildren();
    for (auto &child : children)
    {
        if (child->getRawHandler() == nullptr) continue;

        xml_node childElement = element.append_child("");
        traverse(childElement, child);
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

    const auto automationId = node.attribute("automationid").value();
    const auto len = strlen(automationId);
    if (len > 0) {
        xpath.append("//")
             .append(node.name())
             .append("[@automationid=\"")
             .append(automationId)
             .append("\"]");

        try {
            const auto nodes = doc->select_nodes(xpath.c_str());
            if (nodes.size() > 1) {
                for (const auto& no : nodes) {
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

    auto parent = node.parent();
    if (parent) {
        const xml_node& child = parent.first_child();
        for (auto el = child; el; el = el.next_sibling()) {
            if (el.name() && node.name() && !strcmp(el.name(), node.name())) {
                count++;
                if (el == node) index = count;
            }
        }
    }

    xpath.append("/")
         .append(node.name());

    if (count > 1)
        xpath.append(1, '[')
             .append(std::to_string(index))
             .append(1, ']');

    if (node == doc->first_child()) return xpath;

    return getOptimalXPath(doc, parent).append(xpath);
}

std::string makeQuery(const std::string& id) {
    std::stringstream query;
    query << "//*[@id=\"" << id << "\"]";
    return query.str();
}

std::shared_ptr<AccessibleNode> AurumXML::checkParentNode(const std::shared_ptr<AccessibleNode>& node)
{
    const auto parent = node->getParent();
    if (!parent) return nullptr;

    const std::string query = makeQuery(node->getId());
    xml_node xmlNode = mDoc->select_node(query.c_str()).node();

    if (xmlNode) {
        auto children = node->getChildren();
        for (auto &child : children) {
            if (child->getRawHandler() == nullptr) continue;

            xml_node childElement = xmlNode.append_child("");
            traverse(childElement, child);
        }
        return parent;
    }

    return checkParentNode(parent);
}

xml_node AurumXML::checkNode(const std::shared_ptr<AccessibleNode>& node)
{
    xml_node xmlNode;

    try {
        const std::string query = makeQuery(node->getId());
        xmlNode = mDoc->select_node(query.c_str()).node();

        if (!xmlNode) {
            // 1. find parent and check node again
            auto parent = checkParentNode(node);

            // 2. clear tree and create tree again
            if (!parent) createXMLtree();

            xmlNode = mDoc->select_node(query.c_str()).node();
        }
    } catch (const xpath_exception &e) {
        LOGI("getXPath Error: %s", e.what());
    }
    return xmlNode;
}

std::string AurumXML::getXPath(const std::shared_ptr<AccessibleNode>& node)
{
    xml_node xmlNode = checkNode(node);

    if (xmlNode) {
        std::string xpath = getOptimalXPath(mDoc, xmlNode);
        return xpath;
    }

    return "NotSupported";
}

void AurumXML::findObjects(std::vector<std::shared_ptr<AccessibleNode>> &ret,
    std::string xpath, bool earlyReturn)
{
    createXMLtree();

    LOGI("xpath %s earlyReturn %d", xpath.c_str(), earlyReturn);
    try {
        if (earlyReturn) {
            xml_node    node = mDoc->select_node(xpath.c_str()).node();
            const std::string id(node.attribute("id").value());
            if (mXNodeMap.count(id) > 0) ret.push_back(mXNodeMap[id]);

        } else {
            const auto nodes = mDoc->select_nodes(xpath.c_str());

            for (const auto& no : nodes) {
                const std::string id(no.node().attribute("id").value());

                if (mXNodeMap.count(id) > 0) ret.push_back(mXNodeMap[id]);
            }
        }
    } catch (const xpath_exception &e) {
        LOGI("findObjects Error: %s", e.what());
    }
}
