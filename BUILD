package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cave",
    srcs = ["cave.cc"],
    hdrs = ["cave.h"],
    deps = [":map"],
)

cc_library(
    name = "character",
    srcs = ["character.cc"],
    hdrs = ["character.h"],
    deps = [
        ":map",
        "@libgam//:graphics",
        "@libgam//:spritemap",
    ],
)

cc_library(
    name = "island",
    srcs = ["island.cc"],
    hdrs = [
        "island.h",
        "stb_perlin.h",
    ],
    deps = [":map"],
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
        "@libgam//:graphics",
        "@libgam//:spritemap",
    ],
)

cc_library(
    name = "map_screen",
    srcs = ["map_screen.cc"],
    hdrs = ["map_screen.h"],
    deps = [
        ":cave",
        ":character",
        ":island",
        ":map",
        ":npc",
        "@libgam//:screen",
    ],
)

cc_library(
    name = "npc",
    srcs = ["npc.cc"],
    hdrs = ["npc.h"],
    deps = [
        ":character",
        "@libgam//:backdrop",
        "@libgam//:screen",
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
