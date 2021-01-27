using System;
using System.Collections.Generic;
using System.Text;

namespace SharpTest.libsharptest
{
    interface ISearchable
    {
        bool HasObject(UiSelector selector);

        UiObject FindObject(UiSelector selector);

        List<UiObject> FindObjects(UiSelector selector);
    }
}
