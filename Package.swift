// swift-tools-version:5.1
import PackageDescription

let package = Package(

    name: "DezelCoreUI",

    platforms: [
		.iOS(.v11)
    ],

    products: [
		.library(
			name: "DezelCoreUI",
			targets: ["DezelCoreUI"]
		),
    ],

	targets: [

		.target(
			name: "DezelCoreUI",
			dependencies: [],
			path: "src",
			cSettings: [
				.headerSearchPath("./"),
				.headerSearchPath("./style"),
				.headerSearchPath("./layout")
			],
			cxxSettings: [
				.headerSearchPath("./"),
				.headerSearchPath("./style"),
				.headerSearchPath("./layout")
			]
		)

    ],

    cLanguageStandard: .gnu11,

    cxxLanguageStandard: .cxx1z
)
