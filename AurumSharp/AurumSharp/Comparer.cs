using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    class Comparer
    {
        private UiDevice mDevice { get; set; }
        private UiSelector mSelector { get; set; }
        private bool mEarlyReturn { get; set; }

        private Comparer() { }
        private Comparer(UiDevice device, UiSelector selector, bool earlyReturn)
        {
            mDevice = device;
            mSelector = selector;
            mEarlyReturn = earlyReturn;
        }

        public static AccessibleNode findObject(UiDevice device, UiSelector selector, AccessibleNode root)
        {
            List<AccessibleNode> ret = findObjects(device, selector, root, true);
            if (ret.Count > 0)
                return ret[0];
            else
                return null;
        }

        public static List<AccessibleNode> findObjects(UiDevice device, UiSelector selector, AccessibleNode root, bool earlyReturn = false)
        {
            Comparer comparer = new Comparer(device, selector, earlyReturn);

            if (selector != null && selector.mParent != null)
            {
                var ret = Comparer.findObjects(device, selector.mParent, root);
                List<AccessibleNode> merged = new List<AccessibleNode>();

                foreach (var node in ret)
                {
                    var tmp = comparer.findObjects(node);
                    merged.AddRange(tmp);
                }
                return merged;
            }
            return comparer.findObjects(root);
        }

        private List<AccessibleNode> findObjects(AccessibleNode root)
        {
            LinkedList<PartialMatch> partialList = new LinkedList<PartialMatch>();
            List<AccessibleNode> ret = this.findObjects(root, 0, 0, partialList);
            return ret;
        }

        private List<AccessibleNode> findObjects(AccessibleNode root, int index, int depth, LinkedList<PartialMatch> partialMatches)
        {
            List<AccessibleNode> ret = new List<AccessibleNode>();
            root.refresh();

            foreach (var match in partialMatches)
                match.update(root, index, depth, partialMatches);
            PartialMatch currentMatch = PartialMatch.accept(root, mSelector, index, depth);
            if (currentMatch != null)
            {
                partialMatches.AddFirst(currentMatch);
            }

            if (!(mSelector.mMaxDepth.HasValue && (depth + 1 > mSelector.mMaxDepth.Value)))
            {
                int childCnt = root.getChildCount();
                for (int i = 0; i < childCnt; i++)
                {
                    AccessibleNode childNode = root.getChildAt(i);
                    if (childNode == null || (childNode != null && !childNode.isValid())) continue;

                    List<AccessibleNode> childret = findObjects(childNode, i, depth + 1, partialMatches);
                    ret.AddRange(childret);

                    if (ret.Count > 0 && mEarlyReturn) return ret;
                }
            }
            else
            {
                Tizen.Log.Error("AURUM", "Abort searching! No need to search children(maxDepth limit overflo");
            }
            
            if (currentMatch != null && currentMatch.finalizeMatch())
            {
                //LOG_F(INFO, "Found matched = %s with criteria %s", root->description().c_str(), currentMatch->debugPrint().c_str());
                Tizen.Log.Error("AURUM", "found match : " + root.ToString());
                ret.Add(root);
            }

            return ret;
        }
    }
}
