using System;
using System.Collections.Generic;

namespace SharpTest.libsharptest.Impl.Accessible
{
    internal class AtspiAccessibleApplication : AccessibleApplication
    {
        public AtspiAccessibleApplication(AccessibleNode node) : base(node)
        {
        }

        public override string getPackageName()
        {
            return getAccessibleNode().getText();
        }

        public override List<AccessibleWindow> getWindows()
        {
            List<AccessibleWindow> ret = new List<AccessibleWindow>();
            var children = getAccessibleNode().getChildren();

            children.ForEach(x => {
                ret.Add(new AtspiAccessibleWindow(this, x));
            });

            return ret;
        }
    }
}