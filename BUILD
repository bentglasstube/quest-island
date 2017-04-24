package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cave",
    srcs = ["cave.cc"],
    hdrs = ["cave.h"],
    deps = [
        ":item",
        ":map"
    ],
)

cc_library(
    name = "character",
    srcs = ["character.cc"],
    hdrs = ["character.h"],
    deps = [
        ":item",
        "@libgam//:graphics",
        "@libgam//:spritemap",
    ],
)

cc_library(
    name = "dialog",
    srcs = ["dialog.cc"],
    hdrs = ["dialog.h"],
    deps = [
        "@libgam//:audio",
        "@libgam//:graphics",
        "@libgam//:text",
    ],
)

cc_library(
    name = "island",
    srcs = ["island.cc"],
    hdrs = [
        "island.h",
        "stb_perlin.h",
    ],
    deps = [
        ":cave",
        ":item",
        ":map"
    ],
)

cc_library(
    name = "item",
    srcs = ["item.cc"],
    hdrs = ["item.h"],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
    ],
)

cc_binary(
    name = "ld38",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-lSDL2_ttf",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        ":title_screen",
        "@libgam//:game",
    ],
)

cc_library(
    name = "map",
    srcs = ["map.cc"],
    hdrs = ["map.h"],
    deps = [
        ":character",
        ":item",
        "@libgam//:graphics",
        "@libgam//:spritemap",
        "@libgam//:text",
    ],
)

cc_library(
    name = "map_screen",
    srcs = ["map_screen.cc"],
    hdrs = ["map_screen.h"],
    deps = [
        ":character",
        ":dialog",
        ":island",
        ":item",
        ":map",
        ":pause_menu",
        "@libgam//:screen",
    ],
)

cc_library(
    name = "pause_menu",
    srcs = ["pause_menu.cc"],
    hdrs = ["pause_menu.h"],
    deps = [
        ":item",
        "@libgam//:audio",
        "@libgam//:graphics",
        "@libgam//:input",
        "@libgam//:spritemap",
        "@libgam//:text",
    ],
)

cc_library(
    name = "title_screen",
    srcs = ["title_screen.cc"],
    hdrs = ["title_screen.h"],
    deps = [
        ":map_screen",
        "@libgam//:backdrop",
        "@libgam//:screen",
        "@libgam//:text",
    ],
)
