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

#ifndef _AURUM_XML_H_
#define _AURUM_XML_H_

#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <sstream>

#include "Accessible.h"
#include "config.h"
#include "pugixml.hpp"

using namespace pugi;

namespace Aurum {

/**
 * @class AurumXML
 *
 * @ingroup aurum
 *
 * @brief AurumXML is a representation of a XML document.
 *
 */

class AurumXML {
public:
    /**
     * @brief Construct a new AurumXML object with root node.
     *
     * @since_tizen 7.0
     */
    AurumXML(const std::shared_ptr<AccessibleNode> root, int *appXMLLoadedCount, std::mutex *XMLMutex, std::condition_variable *XMLConditionVar);

    /**
     * @brief Destroy the AurumXML object
     *
     * @since_tizen 7.0
     */
    ~AurumXML();

    /**
     * @brief Creates a XML tree.
     *
     * @return true if XML tree is created successfully, false otherwise
     *
     * @since_tizen 7.0
     */
    bool createXMLtree();

    /**
     * @brief Gets a XPath from specific id.
     *
     * @param node AccessibleNode
     *
     * @return XPath
     *
     * @since_tizen 7.0
     */
    std::string getXPath(const std::shared_ptr<AccessibleNode>& node);

    /**
     * @brief Finds that objects that satisfied with the xpath in the object tree.
     *
     * @param xpath
     *
     * @param ret vector contains objects
     *
     * @since_tizen 7.0
     */
    void findObjects(std::vector<std::shared_ptr<AccessibleNode>> &ret, std::string xpath, bool earlyReturn = false);

private:
    /**
     * @internal
     *
     * @brief Traverse application tree for creating XML tree.
     *
     * @param element xml_node
     *
     * @param node AccessibleNode
     *
     * @since_tizen 7.0
     */
    void traverse(xml_node& element, const std::shared_ptr<AccessibleNode>& node);

    /**
     * @internal
     *
     * @brief Get the Optimal XPath string
     *
     * @param doc xml_document
     *
     * @param node xml_node
     *
     * @return XPath
     *
     * @since_tizen 7.0
     */
    std::string getOptimalXPath(xml_document *doc, xml_node node);

    /**
     * @internal
     *
     * @brief Check xml_node exists or not
     *
     * @param node AccessibleNode
     *
     * @return xml_node
     *
     * @since_tizen 7.0
     */
    xml_node checkNode(const std::shared_ptr<AccessibleNode>& node);

    /**
     * @internal
     *
     * @brief Check parent's xml_node exists or not
     *
     * @param node AccessibleNode
     *
     * @return AccessibleNode
     *
     * @since_tizen 7.5
     */
    std::shared_ptr<AccessibleNode> checkParentNode(const std::shared_ptr<AccessibleNode>& node);

    /**
     * @internal
     *
     * @brief Finds XNode that satisfied with the xpath in the object tree.
     *
     * @param ret vector contains objects
     *
     * @param xpath
     *
     * @param earlyReturn
     *
     * @since_tizen 7.5
     */
    void findXNodes(std::vector<std::shared_ptr<AccessibleNode>> &ret, std::string xpath, bool earlyReturn);

private:

    AurumXML(const AurumXML&) = delete;
    AurumXML& operator=(const AurumXML&) = delete;

    xml_document                                *mDoc;
    const std::shared_ptr<AccessibleNode>        mRoot;
    std::unordered_map<std::string, std::shared_ptr<AccessibleNode>> mXNodeMap;
};
}  // namespace Aurum

#endif
