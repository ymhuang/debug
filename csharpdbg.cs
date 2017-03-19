
        public static void YMHDBG_MSG(String msg)
        {
            String logPath = System.Environment.GetEnvironmentVariable("TEMP");
            if (!logPath.EndsWith("\\"))
            {
                logPath += "\\";
            }

            System.IO.StreamWriter sw = System.IO.File.AppendText(logPath + "ymhdbg.log");
            try
            {
                string logLine = System.String.Format("{0:G}: {1}\n", System.DateTime.Now, msg);
                sw.WriteLine(logLine);
                Console.Out.WriteLine(logLine);
            }
            finally
            {
                sw.Close();
            }
        }

        public static void YMHDBG_ENTRY(String msg)
        {
            YMHDBGMSG("-->" + msg);
        }

        public static void YMHDBG_EXIT(String msg)
        {
            YMHDBGMSG("<--" + msg);
        }


