using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace SharpTest.libsharptest
{
    internal class PartialMatch
    {
        private UiSelector mSelector = null;

        private int mDepth = -1;

        private LinkedList<PartialMatch> mPartialMatches = new LinkedList<PartialMatch>();

        public PartialMatch(UiSelector selector, int depth)
        {
            this.mSelector = selector;
            this.mDepth = depth;
        }

        internal void update(AccessibleNode root, int index, int depth, LinkedList<PartialMatch> partialMatches)
        {
            foreach (var childSelector in mSelector.mChild)
            {
                var match = PartialMatch.accept(root, childSelector, index, depth,
                                                  depth - mDepth);
                if (match != null)
                {
                    mPartialMatches.AddLast(match);
                    partialMatches.AddFirst(match);
                }
            }
        }

        internal static PartialMatch accept(AccessibleNode root, UiSelector mSelector, int index, int depth)
        {
            return PartialMatch.accept(root, mSelector, index, depth, depth);
        }

        static PartialMatch accept(AccessibleNode node, UiSelector selector,
                                                   int index, int absoluteDepth,
                                                   int relativeDepth)
        {
            PartialMatch match = null;

            if ((selector.mMinDepth.HasValue && (relativeDepth < (selector.mMinDepth.Value))) ||
                (selector.mMaxDepth.HasValue && (relativeDepth > (selector.mMaxDepth.Value))))
            {
                return null;
            }
            if (PartialMatch.checkCriteria(selector, node)) match = new PartialMatch(selector, absoluteDepth);
            return match;
        }

        private static bool checkCriteria(UiSelector selector, AccessibleNode node)
        {
            if (selector == null || node == null) return false;

            if (checkCriteria(selector.mText, node.getText(), selector.mMatchText)) return true;
            if (checkCriteria(selector.mId, node.getId(), selector.mMatchId)) return true;
            if (checkCriteria(selector.mAutomationId, node.getAutomationId(), selector.mMatchAutomationId)) return true;
            if (checkCriteria(selector.mType, node.getType(), selector.mMatchType)) return true;
            if (checkCriteria(selector.mStyle, node.getStyle(), selector.mMatchStyle)) return true;
            if (checkCriteria(selector.mPkg, node.getPkg(), selector.mMatchPkg)) return true;
            if (checkCriteria(selector.mRole, node.getRole(), selector.mMatchRole)) return true;
            if (checkCriteria(selector.mIschecked, node.isChecked())) return true;
            if (checkCriteria(selector.mIscheckable, node.isCheckable())) return true;
            if (checkCriteria(selector.mIsclickable, node.isClickable())) return true;
            if (checkCriteria(selector.mIsenabled, node.isEnabled())) return true;
            if (checkCriteria(selector.mIsfocused, node.isFocused())) return true;
            if (checkCriteria(selector.mIsfocusable, node.isFocusable())) return true;
            if (checkCriteria(selector.mIsscrollable, node.isScrollable())) return true;
            if (checkCriteria(selector.mIsselected, node.isSelected())) return true;
            if (checkCriteria(selector.mIsshowing, node.isShowing())) return true;
            if (checkCriteria(selector.mIsactive, node.isActive())) return true;
            if (checkCriteria(selector.mIsvisible, node.isVisible())) return true;
            if (checkCriteria(selector.mIsselectable, node.isSelectable())) return true;

            return false;
        }

        private static bool checkCriteria(string textA, string textB, bool? mMatchText)
        {
            if (textA == null || textA.Length == 0 || mMatchText.HasValue == false) return false;
            
            Regex re = new Regex(textA);
            bool rst = (re.IsMatch(textB) == mMatchText);
            return rst;
        }

        private static bool checkCriteria(string textA, string textB)
        {
            return checkCriteria(textA, textB, true);
        }

        private static bool checkCriteria(bool? boolA, bool boolB)
        {
            if (!boolA.HasValue) return false;
            return boolA == boolB; 
        }

        internal bool finalizeMatch()
        {
            HashSet<UiSelector> matches = new HashSet<UiSelector>();
            
            foreach (var match in mPartialMatches)
            {
                if (match.finalizeMatch())
                {
                    matches.Add(match.mSelector);
                }
            }

            foreach (var sel in mSelector.mChild)
            {
                if (!matches.Contains(sel)) return false;
            }
            return true;
        }
    }
}