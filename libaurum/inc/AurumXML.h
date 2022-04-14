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
    AurumXML(std::shared_ptr<AccessibleNode> root);

    /**
     * @brief Destroy the AurumXML object
     *
     * @since_tizen 7.0
     */
    ~AurumXML();

    /**
     * @brief Creates a XML tree.
     *
     * @return true if XML tree is created succesfully, false otherwise
     *
     * @since_tizen 7.0
     */
    bool createXMLtree();

    /**
     * @brief Gets a XPath from specific id.
     *
     * @param id
     *
     * @return XPath
     *
     * @since_tizen 7.0
     */
    std::string getXPath(std::string id);

    /**
     * @brief Finds that objects that satisfied with the xpath in the object tree.
     *
     * @param xpath
     *
     * @return the list of found AccessibleNode vector
     *
     * @since_tizen 7.0
     */
    std::vector<std::shared_ptr<AccessibleNode>> findObjects(std::string xpath, bool earlyReturn = false);

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
    void traverse(xml_node element, std::shared_ptr<AccessibleNode> node);

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
     * @param string id
     *
     * @return xml_node
     *
     * @since_tizen 7.0
     */
    xml_node checkNode(std::string id);

private:
    xml_document                                *mDoc;
    std::shared_ptr<AccessibleNode>              mRoot;
    std::unordered_map<std::string, std::shared_ptr<AccessibleNode>> mXNodeMap;
};
}  // namespace Aurum

#endif
