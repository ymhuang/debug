package main

import (
    "fmt"
    "golang.org/x/sys/windows/registry"
    "os"
    "os/exec"
    "bufio"
    "time"
    "strconv"
)

func WaitForEnter() {
    fmt.Println("Press ENTER to continue")
    bufio.NewReader(os.Stdin).ReadBytes('\n')
}

func ReadModTime( path string ) {
    fi, err := os.Stat(path)
    if (err != nil) {
        fmt.Println(err.Error())
        os.Exit(0)
    }
    fmt.Println(fi.ModTime().Format(time.RFC3339))
}

func CreateAutoRun( count int ) {
    key, err := registry.OpenKey(registry.LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        registry.ALL_ACCESS|registry.WOW64_64KEY)
    if (err != nil) {
        fmt.Println(err.Error())
        os.Exit(0)
    }
    defer key.Close()

    parameter := fmt.Sprintf("C:\\Autotest.exe %d", count)
    err = key.SetStringValue("Autotest", parameter )
    if (err != nil) {
        fmt.Println(err.Error())
        os.Exit(0)
    }
}

func Reboot() {
    _, err := exec.Command("cmd", "/C shutdown.exe /r -t 00").Output()
    if (err != nil) {
        fmt.Println(err.Error())
    }
}

func main() {

    fmt.Println(os.Args[1], "times" )
    count, _ := strconv.Atoi(os.Args[1])

    ReadModTime("C:\\Windows\\System32\\winver.exe")

    fmt.Println("Create AutoRun Registry Key")
    CreateAutoRun(count+1)

    fmt.Println("Reboot")
//    WaitForEnter()
    Reboot()
}
