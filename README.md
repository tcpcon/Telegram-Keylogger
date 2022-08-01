# Telegram Keylogger
A small windows keylogger with **NO DEPENDENCIES** made in C/Winapi, logs are sent to a telegram channel set by the attacker. Program does not have features for persistence or any antivirus bypassing techniques meaning it is quite ineffective at staying hidden in the wild. Keylogger was made to showcase how SetWindowsHook can be used malicously to capture keystrokes and how easy it is to send data to a cnc which in this case is our telegram channel. The keylogger when compiled is ~15kb.

## Compile
> Charset **MUST** be set to multi-byte

Edit "Config.h" with TG_BOT_TOKEN, TG_CHAT_ID AND SEND_DELAY before compiling with Visual Studio.
