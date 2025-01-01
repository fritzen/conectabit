using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Flightgear.Control
{
    class CsvData
    {
        public string name;
        public int[] values;


        public string Name
        {
            get { return name; }
            set
            {
                name = value;
            }

        }

        public String GetSensorValues()
        {
            String v = "";
            foreach (int i in values)
            {
                v = v + "," + i;
            }
            return v;
        }

}
}
