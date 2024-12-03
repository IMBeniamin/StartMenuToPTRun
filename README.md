## This app aims to be extremely lightweight and run in the background to remap the meta (aka windows) key to the alt+space combo

<br>

## !! _**Absolutely no data leaves your machine**_ !!

You can grab the latest release from the releases page. (Please check the sources first).

Release builds have file logging activated by default. Check the logging section for more information.

<br>

## At the moment, no GUI or tray icon is available. This will likely change in the future.

To stop the process, open task manager (Ctrl + Shift + Esc), search for the executable name and end the task.

When running the program, nothing will show up on your screen. (In the future a tray icon may be added)


<details open>

<summary>
Logging tips and information
</summary>

### Logging

A very lightweight logger has been added.

It can be configured by using the LogConfig object to specify the level and the destination of the logs.
If the file logging is enabled, the logs will be saved in the following directory:
`%appdata%\KeyboardHook\log_<timestamp>.log`
A new log file will be created every time the application is started.
</details>



