{
    "patcher": {
        "fileversion": 1,
        "appversion": {
            "major": 8,
            "minor": 0,
            "revision": 6,
            "architecture": "x64",
            "modernui": 1
        },
        "classnamespace": "box",
        "rect": [
            34.0,
            79.0,
            662.0,
            793.0
        ],
        "bglocked": 0,
        "openinpresentation": 0,
        "default_fontsize": 12.0,
        "default_fontface": 0,
        "default_fontname": "Arial",
        "gridonopen": 1,
        "gridsize": [
            15.0,
            15.0
        ],
        "gridsnaponopen": 1,
        "objectsnaponopen": 1,
        "statusbarvisible": 2,
        "toolbarvisible": 1,
        "lefttoolbarpinned": 0,
        "toptoolbarpinned": 0,
        "righttoolbarpinned": 0,
        "bottomtoolbarpinned": 0,
        "toolbars_unpinned_last_save": 0,
        "tallnewobj": 0,
        "boxanimatetime": 200,
        "enablehscroll": 1,
        "enablevscroll": 1,
        "devicewidth": 0.0,
        "description": "",
        "digest": "",
        "tags": "",
        "style": "",
        "subpatcher_template": "nobars",
        "showrootpatcherontab": 0,
        "showontab": 0,
        "boxes": [
            {
                "box": {
                    "id": "obj-2",
                    "maxclass": "newobj",
                    "numinlets": 0,
                    "numoutlets": 0,
                    "patcher": {
                        "fileversion": 1,
                        "appversion": {
                            "major": 8,
                            "minor": 0,
                            "revision": 6,
                            "architecture": "x64",
                            "modernui": 1
                        },
                        "classnamespace": "box",
                        "rect": [
                            34.0,
                            105.0,
                            662.0,
                            767.0
                        ],
                        "bglocked": 0,
                        "openinpresentation": 0,
                        "default_fontsize": 12.0,
                        "default_fontface": 0,
                        "default_fontname": "Arial",
                        "gridonopen": 2,
                        "gridsize": [
                            15.0,
                            15.0
                        ],
                        "gridsnaponopen": 2,
                        "objectsnaponopen": 1,
                        "statusbarvisible": 2,
                        "toolbarvisible": 1,
                        "lefttoolbarpinned": 0,
                        "toptoolbarpinned": 0,
                        "righttoolbarpinned": 0,
                        "bottomtoolbarpinned": 0,
                        "toolbars_unpinned_last_save": 0,
                        "tallnewobj": 0,
                        "boxanimatetime": 200,
                        "enablehscroll": 1,
                        "enablevscroll": 1,
                        "devicewidth": 0.0,
                        "description": "",
                        "digest": "",
                        "tags": "",
                        "style": "",
                        "subpatcher_template": "nobars",
                        "showontab": 1,
                        "boxes": [
                            {
                                "box": {
                                    "hidden": 1,
                                    "id": "obj-1",
                                    "linecount": 7,
                                    "maxclass": "newobj",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        "bang"
                                    ],
                                    "patching_rect": [
                                        626.0,
                                        63.0,
                                        19.0,
                                        102.0
                                    ],
                                    "text": "loadbang"
                                }
                            },
                            {
                                "box": {
                                    "border": 0,
                                    "filename": "fl.helpdetails.js",
                                    "id": "obj-4",
                                    "ignoreclick": 1,
                                    "jsarguments": "fl.atanh~",
                                    "maxclass": "jsui",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        ""
                                    ],
                                    "parameter_enable": 0,
                                    "patching_rect": [
                                        15.0,
                                        15.0,
                                        630.0,
                                        150.0
                                    ]
                                }
                            },
                            {
                                "box": {
                                    "fontsize": 13.0,
                                    "id": "obj-3",
                                    "maxclass": "comment",
                                    "numinlets": 1,
                                    "numoutlets": 0,
                                    "patching_rect": [
                                        400.0,
                                        735.0,
                                        245.0,
                                        21.0
                                    ],
                                    "text": "Traverse the tabs for more information!",
                                    "textjustification": 2
                                }
                            },
                            {
                                "box": {
                                    "bgmode": 0,
                                    "border": 0,
                                    "clickthrough": 0,
                                    "enablehscroll": 0,
                                    "enablevscroll": 0,
                                    "id": "obj-128",
                                    "lockeddragscroll": 0,
                                    "maxclass": "bpatcher",
                                    "name": "fldocs.getparams.maxpat",
                                    "numinlets": 1,
                                    "numoutlets": 0,
                                    "offset": [
                                        0.0,
                                        0.0
                                    ],
                                    "patching_rect": [
                                        15.0,
                                        167.0,
                                        630.0,
                                        565.0
                                    ],
                                    "viewvisibility": 1
                                }
                            }
                        ],
                        "lines": [
                            {
                                "patchline": {
                                    "destination": [
                                        "obj-128",
                                        0
                                    ],
                                    "hidden": 1,
                                    "midpoints": [
                                        635.5,
                                        168.0,
                                        657.0,
                                        168.0,
                                        657.0,
                                        166.0,
                                        24.5,
                                        166.0
                                    ],
                                    "source": [
                                        "obj-1",
                                        0
                                    ]
                                }
                            }
                        ]
                    },
                    "patching_rect": [
                        2.0,
                        5.0,
                        38.0,
                        22.0
                    ],
                    "saved_object_attributes": {
                        "description": "",
                        "digest": "",
                        "globalpatchername": "",
                        "tags": ""
                    },
                    "text": "p info"
                }
            },
            {
                "box": {
                    "id": "obj-2",
                    "maxclass": "newobj",
                    "numinlets": 0,
                    "numoutlets": 0,
                    "patcher": {
                        "fileversion": 1,
                        "appversion": {
                            "major": 8,
                            "minor": 0,
                            "revision": 6,
                            "architecture": "x64",
                            "modernui": 1
                        },
                        "classnamespace": "box",
                        "rect": [
                            0.0,
                            26.0,
                            652.0,
                            761.0
                        ],
                        "bglocked": 0,
                        "openinpresentation": 0,
                        "default_fontsize": 12.0,
                        "default_fontface": 0,
                        "default_fontname": "Arial",
                        "gridonopen": 1,
                        "gridsize": [
                            15.0,
                            15.0
                        ],
                        "gridsnaponopen": 1,
                        "objectsnaponopen": 1,
                        "statusbarvisible": 2,
                        "toolbarvisible": 1,
                        "lefttoolbarpinned": 0,
                        "toptoolbarpinned": 0,
                        "righttoolbarpinned": 0,
                        "bottomtoolbarpinned": 0,
                        "toolbars_unpinned_last_save": 0,
                        "tallnewobj": 0,
                        "boxanimatetime": 200,
                        "enablehscroll": 1,
                        "enablevscroll": 1,
                        "devicewidth": 0.0,
                        "description": "",
                        "digest": "",
                        "tags": "",
                        "style": "",
                        "subpatcher_template": "",
                        "showontab": 1,
                        "boxes": [],
                        "lines": []
                    },
                    "patching_rect": [
                        540.0,
                        230.0,
                        25.0,
                        22.0
                    ],
                    "saved_object_attributes": {
                        "description": "",
                        "digest": "",
                        "globalpatchername": "",
                        "tags": ""
                    },
                    "text": "p ?"
                }
            },
            {
                "box": {
                    "id": "obj-1",
                    "maxclass": "newobj",
                    "numinlets": 0,
                    "numoutlets": 0,
                    "patcher": {
                        "fileversion": 1,
                        "appversion": {
                            "major": 8,
                            "minor": 0,
                            "revision": 6,
                            "architecture": "x64",
                            "modernui": 1
                        },
                        "classnamespace": "box",
                        "rect": [
                            34.0,
                            105.0,
                            652.0,
                            761.0
                        ],
                        "bglocked": 0,
                        "openinpresentation": 0,
                        "default_fontsize": 13.0,
                        "default_fontface": 0,
                        "default_fontname": "Arial",
                        "gridonopen": 2,
                        "gridsize": [
                            15.0,
                            15.0
                        ],
                        "gridsnaponopen": 2,
                        "objectsnaponopen": 1,
                        "statusbarvisible": 2,
                        "toolbarvisible": 1,
                        "lefttoolbarpinned": 0,
                        "toptoolbarpinned": 0,
                        "righttoolbarpinned": 0,
                        "bottomtoolbarpinned": 0,
                        "toolbars_unpinned_last_save": 0,
                        "tallnewobj": 0,
                        "boxanimatetime": 200,
                        "enablehscroll": 1,
                        "enablevscroll": 1,
                        "devicewidth": 0.0,
                        "description": "",
                        "digest": "",
                        "tags": "",
                        "style": "",
                        "subpatcher_template": "",
                        "showontab": 1,
                        "boxes": [
                            {
                                "box": {
                                    "id": "obj-4",
                                    "maxclass": "ezdac~",
                                    "numinlets": 2,
                                    "numoutlets": 0,
                                    "patching_rect": [
                                        15.0,
                                        375.0,
                                        45.0,
                                        45.0
                                    ]
                                }
                            },
                            {
                                "box": {
                                    "format": 6,
                                    "id": "obj-30",
                                    "maxclass": "flonum",
                                    "numinlets": 1,
                                    "numoutlets": 2,
                                    "outlettype": [
                                        "",
                                        "bang"
                                    ],
                                    "parameter_enable": 0,
                                    "patching_rect": [
                                        15.0,
                                        330.0,
                                        50.0,
                                        23.0
                                    ]
                                }
                            },
                            {
                                "box": {
                                    "format": 6,
                                    "id": "obj-28",
                                    "maxclass": "flonum",
                                    "numinlets": 1,
                                    "numoutlets": 2,
                                    "outlettype": [
                                        "",
                                        "bang"
                                    ],
                                    "parameter_enable": 0,
                                    "patching_rect": [
                                        15.0,
                                        150.0,
                                        50.0,
                                        23.0
                                    ]
                                }
                            },
                            {
                                "box": {
                                    "id": "obj-26",
                                    "maxclass": "newobj",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        ""
                                    ],
                                    "patching_rect": [
                                        15.0,
                                        105.0,
                                        86.0,
                                        23.0
                                    ],
                                    "text": "loadmess 0.8"
                                }
                            },
                            {
                                "box": {
                                    "arrows": 1,
                                    "border": 4.0,
                                    "id": "obj-38",
                                    "maxclass": "live.line",
                                    "numinlets": 1,
                                    "numoutlets": 0,
                                    "patching_rect": [
                                        94.0,
                                        199.0,
                                        60.25,
                                        15.0
                                    ]
                                }
                            },
                            {
                                "box": {
                                    "id": "obj-31",
                                    "maxclass": "newobj",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        ""
                                    ],
                                    "patching_rect": [
                                        15.0,
                                        240.0,
                                        59.0,
                                        23.0
                                    ],
                                    "text": "fl.atanh~"
                                }
                            },
                            {
                                "box": {
                                    "id": "obj-20",
                                    "maxclass": "newobj",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        ""
                                    ],
                                    "patching_rect": [
                                        15.0,
                                        284.75,
                                        62.0,
                                        23.0
                                    ],
                                    "text": "fl.tomax~"
                                }
                            },
                            {
                                "box": {
                                    "id": "obj-13",
                                    "maxclass": "newobj",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        ""
                                    ],
                                    "patching_rect": [
                                        106.5,
                                        105.0,
                                        75.0,
                                        23.0
                                    ],
                                    "text": "fl.perblock~"
                                }
                            },
                            {
                                "box": {
                                    "id": "obj-12",
                                    "maxclass": "newobj",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        ""
                                    ],
                                    "patching_rect": [
                                        15.0,
                                        195.0,
                                        77.0,
                                        23.0
                                    ],
                                    "text": "fl.frommax~"
                                }
                            },
                            {
                                "box": {
                                    "arrows": 1,
                                    "border": 4.0,
                                    "id": "obj-24",
                                    "maxclass": "live.line",
                                    "numinlets": 1,
                                    "numoutlets": 0,
                                    "patching_rect": [
                                        68.5,
                                        333.0,
                                        60.25,
                                        15.0
                                    ]
                                }
                            },
                            {
                                "box": {
                                    "id": "obj-25",
                                    "linecount": 2,
                                    "maxclass": "comment",
                                    "numinlets": 1,
                                    "numoutlets": 0,
                                    "patching_rect": [
                                        130.75,
                                        322.5,
                                        185.0,
                                        36.0
                                    ],
                                    "text": "Hyperbolic arctangent of input frame's values"
                                }
                            },
                            {
                                "box": {
                                    "id": "obj-23",
                                    "maxclass": "comment",
                                    "numinlets": 1,
                                    "numoutlets": 0,
                                    "patching_rect": [
                                        156.25,
                                        196.0,
                                        211.0,
                                        21.0
                                    ],
                                    "text": "Frame containing values to test"
                                }
                            },
                            {
                                "box": {
                                    "border": 0,
                                    "filename": "fl.helpname.js",
                                    "id": "obj-3",
                                    "ignoreclick": 1,
                                    "jsarguments": [
                                        "fl.atanh~"
                                    ],
                                    "maxclass": "jsui",
                                    "numinlets": 1,
                                    "numoutlets": 1,
                                    "outlettype": [
                                        ""
                                    ],
                                    "parameter_enable": 0,
                                    "patching_rect": [
                                        15.0,
                                        15.0,
                                        435.0,
                                        75.0
                                    ]
                                }
                            }
                        ],
                        "lines": [
                            {
                                "patchline": {
                                    "destination": [
                                        "obj-31",
                                        0
                                    ],
                                    "source": [
                                        "obj-12",
                                        0
                                    ]
                                }
                            },
                            {
                                "patchline": {
                                    "destination": [
                                        "obj-12",
                                        0
                                    ],
                                    "midpoints": [
                                        116.0,
                                        180.0,
                                        24.5,
                                        180.0
                                    ],
                                    "source": [
                                        "obj-13",
                                        0
                                    ]
                                }
                            },
                            {
                                "patchline": {
                                    "destination": [
                                        "obj-30",
                                        0
                                    ],
                                    "source": [
                                        "obj-20",
                                        0
                                    ]
                                }
                            },
                            {
                                "patchline": {
                                    "destination": [
                                        "obj-28",
                                        0
                                    ],
                                    "source": [
                                        "obj-26",
                                        0
                                    ]
                                }
                            },
                            {
                                "patchline": {
                                    "destination": [
                                        "obj-12",
                                        0
                                    ],
                                    "source": [
                                        "obj-28",
                                        0
                                    ]
                                }
                            },
                            {
                                "patchline": {
                                    "destination": [
                                        "obj-20",
                                        0
                                    ],
                                    "source": [
                                        "obj-31",
                                        0
                                    ]
                                }
                            }
                        ],
                        "styles": [
                            {
                                "name": "newobjBlue-1",
                                "default": {
                                    "accentcolor": [
                                        0.317647,
                                        0.654902,
                                        0.976471,
                                        1.0
                                    ]
                                },
                                "parentstyle": "",
                                "multi": 0
                            },
                            {
                                "name": "newobjYellow-1",
                                "default": {
                                    "accentcolor": [
                                        0.82517,
                                        0.78181,
                                        0.059545,
                                        1.0
                                    ],
                                    "fontsize": [
                                        12.059008
                                    ]
                                },
                                "parentstyle": "",
                                "multi": 0
                            },
                            {
                                "name": "numberGold-1",
                                "default": {
                                    "accentcolor": [
                                        0.764706,
                                        0.592157,
                                        0.101961,
                                        1.0
                                    ]
                                },
                                "parentstyle": "",
                                "multi": 0
                            }
                        ]
                    },
                    "patching_rect": [
                        271.0,
                        230.0,
                        47.0,
                        22.0
                    ],
                    "saved_object_attributes": {
                        "description": "",
                        "digest": "",
                        "fontsize": 13.0,
                        "globalpatchername": "",
                        "tags": ""
                    },
                    "text": "p basic"
                }
            }
        ],
        "lines": [],
        "dependency_cache": [
            {
                "name": "fldocs.getparams.maxpat",
                "bootpath": "~/FrameLib/Current Test Version/FrameLib/misc",
                "patcherrelativepath": "../../../Current Test Version/FrameLib/misc",
                "type": "JSON",
                "implicit": 1
            },
            {
                "name": "fl.getname.js",
                "bootpath": "~/FrameLib/Current Test Version/FrameLib/misc",
                "patcherrelativepath": "../../../Current Test Version/FrameLib/misc",
                "type": "TEXT",
                "implicit": 1
            },
            {
                "name": "fl.helpdetails.js",
                "bootpath": "~/FrameLib/Current Test Version/FrameLib/misc",
                "patcherrelativepath": "../../../Current Test Version/FrameLib/misc",
                "type": "TEXT",
                "implicit": 1
            }
        ],
        "autosave": 0
    }
}