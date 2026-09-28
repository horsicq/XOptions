Options for projects:

* Detect It Easy: https://github.com/horsicq/Detect-It-Easy
* XAPKDetector: https://github.com/horsicq/XAPKDetector
* XELFViewer: https://github.com/horsicq/XELFViewer
* XPEViewer: https://github.com/horsicq/XPEViewer
* XMACHOViewer: https://github.com/horsicq/XMACHOViewer
* PDBRipper: https://github.com/horsicq/PDBRipper
* XNTSV: https://github.com/horsicq/xntsv
* XDepends: https://github.com/horsicq/XDepends

Console output uses the sibling `xxfclib` terminal module. `XColorString`
keeps the color rules, parsing and ANSI/native color mapping in XOptions;
the C module writes text and restores platform console state.
`xx_set_color_output_enabled(false)` disables colors globally. Redirected
output stays plain, and `XConsoleOutput` sends errors to stderr and warnings
and information to stdout.

The CMake and qmake includes compile the terminal/settings/global runtime subset
automatically. If the parent already links xxfclib, set
`XOPTIONS_USE_EXTERNAL_XXFCLIB` in CMake or add `xxfclib_external` to qmake's
`XCONFIG`, and use the parent library's include paths, definitions and linkage.

Application settings use the C `xx_settings` store. The global pointer starts
null: `load()` applies defaults, `save()` performs no persistence, and
`isWritable()` returns false. Explicitly enable persistence before loading:

```cpp
XOptions options;
options.addID(XOptions::ID_AUTHUSER, QString());
if (options.initializeSettings()) {
    options.load();
    // Change option values, then call options.save().
}
```

`initializeSettings()` keeps an already attached store, otherwise creates an
XOptions-owned native or portable INI store using the existing `isNative()` and
`setName()` policy. It does not load or save. The owner destroys its store when
XOptions is destroyed; `getSettings()` returns a borrowed pointer. Keep that
owner alive while other XOptions instances use the store.

For an explicit location/format, call `XOptions::setSettings()` with a store
created by `xx_settings_create_ini()`, `xx_settings_create_native()`, or
`xx_settings_create_memory()`. An externally attached store remains caller-owned.
`setSettings(nullptr)` disables persistence. Existing applications must enable
settings explicitly; constructing XOptions alone no longer creates storage.

Option defaults, path validation and history policy stay in XOptions. Common
QVariant values map to the C types; other QVariant values use opaque QDataStream
data with the Qt 5.0 stream version. Load existing settings before their first
save to migrate historical QSettings INI/registry values, including recent-file
lists and binary/variant records. Legacy QSettings access is read-only. Load/save
errors emit `errorMessage`; failed loads preserve the C store's previous values.

`test/XSettings` exercises defaults without persistence, store ownership, typed
INI values, legacy migration, history policy and error preservation.
