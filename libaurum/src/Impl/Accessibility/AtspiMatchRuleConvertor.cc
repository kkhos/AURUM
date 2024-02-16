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


#include <atspi/atspi.h>

#include "Aurum.h"
#include "AtspiMatchRuleConvertor.h"

using namespace Aurum;
using namespace AurumInternal;

AtspiMatchRuleConvertor::AtspiMatchRuleConvertor(const std::shared_ptr<UiSelector> selector)
: mSelector(selector)
{
}

AtspiMatchRuleConvertor::~AtspiMatchRuleConvertor()
{
}

AtspiMatchRuleConvertor::operator AtspiMatchRule *()
{
    if (!mSelector) return nullptr;

    AtspiCollectionMatchType stateMatchType = ATSPI_Collection_MATCH_INVALID;
    AtspiCollectionMatchType attributeMatchType = ATSPI_Collection_MATCH_INVALID;

    // Add rule of states
    AtspiStateSet *ss = atspi_state_set_new(nullptr);
    if (mSelector->mIschecked) atspi_state_set_add(ss, ATSPI_STATE_CHECKED);
    if (mSelector->mIscheckable) atspi_state_set_add(ss, ATSPI_STATE_CHECKABLE);
    if (mSelector->mIsclickable) atspi_state_set_add(ss, ATSPI_STATE_SENSITIVE);
    if (mSelector->mIsenabled) atspi_state_set_add(ss, ATSPI_STATE_ENABLED);
    if (mSelector->mIsfocused) atspi_state_set_add(ss, ATSPI_STATE_FOCUSED);
    if (mSelector->mIsfocusable) atspi_state_set_add(ss, ATSPI_STATE_FOCUSABLE);
    if (mSelector->mIsselected) atspi_state_set_add(ss, ATSPI_STATE_SELECTED);
    if (mSelector->mIsshowing) atspi_state_set_add(ss, ATSPI_STATE_SHOWING);
    if (mSelector->mIsactive) atspi_state_set_add(ss, ATSPI_STATE_ACTIVE);
    if (mSelector->mIsvisible) atspi_state_set_add(ss, ATSPI_STATE_VISIBLE);
    if (mSelector->mIsselectable) atspi_state_set_add(ss, ATSPI_STATE_SELECTABLE);
    if (mSelector->mIshighlightable) atspi_state_set_add(ss, ATSPI_STATE_HIGHLIGHTABLE);

    if (atspi_state_set_is_empty(ss))
    {
        g_object_unref(ss);
        ss = nullptr;
    }
    else
    {
        stateMatchType = ATSPI_Collection_MATCH_ALL;
    }

    // Add rule of attributes
    GHashTable *attributes = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    if (mSelector->mMatchType)
    {
        g_hash_table_insert(attributes, g_strdup("class"), g_strdup(mSelector->mType.c_str()));
    }

    if (mSelector->mMatchAutomationId)
    {
        g_hash_table_insert(attributes, g_strdup("automationId"), g_strdup(mSelector->mAutomationId.c_str()));
    }

    if (g_hash_table_size(attributes) == 0)
    {
        g_hash_table_unref (attributes);
        attributes = nullptr;
    }
    else
    {
        attributeMatchType = ATSPI_Collection_MATCH_ALL;
    }

    AtspiMatchRule* rule = atspi_match_rule_new(ss, stateMatchType,
                                                attributes, attributeMatchType,
                                                nullptr, ATSPI_Collection_MATCH_INVALID,
                                                nullptr, ATSPI_Collection_MATCH_INVALID,
                                                false);

    if (ss) g_object_unref(ss);
    if (attributes) g_hash_table_unref (attributes);

    return rule;
}
