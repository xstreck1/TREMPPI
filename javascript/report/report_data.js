var report_data = {
	"setup": {
		"comparative": true,
		"compare": "(NOT (Rosen AND Rosen AND RosenB AND Triv_Fixpoint AND CycAttractor_MAPK AND CycAttractor_mTor AND  Mut_FP))",
		"compared": 12986,
		"date": "Thu Jul 03 12:07:36 2014",
		"name": "crossfunc",
		"pool_size": 13266,
		"select": "(Rosen AND Rosen AND RosenB AND Triv_Fixpoint AND CycAttractor_MAPK AND CycAttractor_mTor AND  Mut_FP)",
		"selected": 280
	},
	"Regulatory": {
		"select": {
			"elements": {
				"nodes": [
					{
						"data": {
							"id": "Akt",
							"name": "Akt"
						},
						"position": {
							"x": 68.5,
							"y": 51.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Erk",
							"name": "Erk"
						},
						"position": {
							"x": 205.5,
							"y": 51.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Mek",
							"name": "Mek"
						},
						"position": {
							"x": 68.5,
							"y": 155.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "PI3K",
							"name": "PI3K"
						},
						"position": {
							"x": 205.5,
							"y": 155.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "RTK",
							"name": "RTK"
						},
						"position": {
							"x": 68.5,
							"y": 258.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Raf",
							"name": "Raf"
						},
						"position": {
							"x": 205.5,
							"y": 258.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Tsc",
							"name": "Tsc"
						},
						"position": {
							"x": 68.5,
							"y": 362.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "mTor",
							"name": "mTor"
						},
						"position": {
							"x": 205.5,
							"y": 362.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					}
				],
				"edges": [
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "PI3K",
							"target": "Akt",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e0"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "Mek",
							"target": "Erk",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e1"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "Raf",
							"target": "Mek",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e2"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.8181818181818182,
							"pearson": -0.3489016221354863,
							"source": "Erk",
							"target": "PI3K",
							"color_mapper": 0.3489016221354863,
							"width_mapper": 0.8181818181818182,
							"target_arrow_shape": "tee",
							"id": "e3"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 0.4538743983518276,
							"source": "RTK",
							"target": "PI3K",
							"color_mapper": 0.4538743983518276,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e4"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.4538743983518208,
							"source": "mTor",
							"target": "PI3K",
							"color_mapper": 0.4538743983518208,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e5"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "RTK",
							"target": "RTK",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e6"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.926829268292683,
							"pearson": -0.3442451657755377,
							"source": "Akt",
							"target": "Raf",
							"color_mapper": 0.3442451657755377,
							"width_mapper": 0.926829268292683,
							"target_arrow_shape": "tee",
							"id": "e7"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.3796634992642713,
							"source": "Erk",
							"target": "Raf",
							"color_mapper": 0.3796634992642713,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e8"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.9105691056910569,
							"pearson": 0.3102120148567476,
							"source": "PI3K",
							"target": "Raf",
							"color_mapper": 0.3102120148567476,
							"width_mapper": 0.9105691056910569,
							"target_arrow_shape": "triangle",
							"id": "e9"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 0.3796634992642704,
							"source": "RTK",
							"target": "Raf",
							"color_mapper": 0.3796634992642704,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e10"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.7182335127931944,
							"source": "Akt",
							"target": "Tsc",
							"color_mapper": 0.7182335127931944,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e11"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.6666666666666666,
							"pearson": -0.3849001794597454,
							"source": "Erk",
							"target": "Tsc",
							"color_mapper": 0.3849001794597454,
							"width_mapper": 0.6666666666666666,
							"target_arrow_shape": "tee",
							"id": "e12"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.6666666666666666,
							"pearson": 0.3849001794597454,
							"source": "Erk",
							"target": "mTor",
							"color_mapper": 0.3849001794597454,
							"width_mapper": 0.6666666666666666,
							"target_arrow_shape": "triangle",
							"id": "e13"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.7182335127931944,
							"source": "Tsc",
							"target": "mTor",
							"color_mapper": 0.7182335127931944,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e14"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					}
				]
			},
			"style": [
				{
					"selector": "node, edge",
					"css": {
						"text-valign": "top",
						"text-halign": "center",
						"color": "#000",
						"content": "",
						"text-outline-color": "#000",
						"text-outline-width": "0px",
						"text-outline-opacity": "1",
						"text-opacity": "1",
						"text-decoration": "none",
						"text-transform": "none",
						"font-family": "Helvetica",
						"font-style": "normal",
						"font-variant": "normal",
						"font-weight": "normal",
						"font-size": "16px",
						"min-zoomed-font-size": "0px",
						"display": "element",
						"visibility": "visible",
						"opacity": "1",
						"z-index": "0",
						"overlay-padding": "10px",
						"overlay-color": "#000",
						"overlay-opacity": "0",
						"transition-property": "none",
						"transition-duration": "0s",
						"transition-delay": "0s",
						"background-blacken": "0",
						"background-color": "#888",
						"background-opacity": "1",
						"background-image": "none",
						"background-position-x": "50%",
						"background-position-y": "50%",
						"background-repeat": "no-repeat",
						"background-fit": "none",
						"background-clip": "node",
						"pie-size": "100%",
						"pie-1-background-color": "black",
						"pie-2-background-color": "black",
						"pie-3-background-color": "black",
						"pie-4-background-color": "black",
						"pie-5-background-color": "black",
						"pie-6-background-color": "black",
						"pie-7-background-color": "black",
						"pie-8-background-color": "black",
						"pie-9-background-color": "black",
						"pie-10-background-color": "black",
						"pie-11-background-color": "black",
						"pie-12-background-color": "black",
						"pie-13-background-color": "black",
						"pie-14-background-color": "black",
						"pie-15-background-color": "black",
						"pie-16-background-color": "black",
						"pie-1-background-size": "0%",
						"pie-2-background-size": "0%",
						"pie-3-background-size": "0%",
						"pie-4-background-size": "0%",
						"pie-5-background-size": "0%",
						"pie-6-background-size": "0%",
						"pie-7-background-size": "0%",
						"pie-8-background-size": "0%",
						"pie-9-background-size": "0%",
						"pie-10-background-size": "0%",
						"pie-11-background-size": "0%",
						"pie-12-background-size": "0%",
						"pie-13-background-size": "0%",
						"pie-14-background-size": "0%",
						"pie-15-background-size": "0%",
						"pie-16-background-size": "0%",
						"border-color": "#000",
						"border-opacity": "1",
						"border-width": "0px",
						"border-style": "solid",
						"height": "30px",
						"width": "30px",
						"padding-left": "0px",
						"padding-right": "0px",
						"padding-top": "0px",
						"padding-bottom": "0px",
						"shape": "ellipse",
						"source-arrow-shape": "none",
						"target-arrow-shape": "none",
						"source-arrow-color": "#bbb",
						"target-arrow-color": "#bbb",
						"source-arrow-fill": "filled",
						"target-arrow-fill": "filled",
						"line-style": "solid",
						"line-color": "#bbb",
						"control-point-step-size": "40px",
						"control-point-weight": "0.5",
						"curve-style": "bezier"
					}
				},
				{
					"selector": "node > node",
					"css": {
						"background-opacity": "0.5",
						"height": "auto",
						"width": "auto",
						"padding-left": "10px",
						"padding-right": "10px",
						"padding-top": "10px",
						"padding-bottom": "10px",
						"shape": "rectangle"
					}
				},
				{
					"selector": "edge",
					"css": {
						"width": "1px"
					}
				},
				{
					"selector": ":active",
					"css": {
						"overlay-padding": "10px",
						"overlay-color": "black",
						"overlay-opacity": "0.25"
					}
				},
				{
					"selector": "core",
					"css": {
						"selection-box-color": "#ddd",
						"selection-box-opacity": "0.65",
						"selection-box-border-color": "#aaa",
						"selection-box-border-width": "1px",
						"panning-cursor": "grabbing",
						"active-bg-color": "black",
						"active-bg-opacity": "0.15",
						"active-bg-size": "15px",
						"outside-texture-bg-color": "#000",
						"outside-texture-bg-opacity": "0.125"
					}
				},
				{
					"selector": "node",
					"css": {
						"text-valign": "center",
						"content": "data(name)",
						"background-color": "white",
						"border-color": "black",
						"border-width": "2px",
						"height": "50px",
						"width": "50px"
					}
				},
				{
					"selector": "edge",
					"css": {
						"target-arrow-shape": "data(target_arrow_shape)",
						"target-arrow-color": "black",
						"line-style": "data(line_style)"
					}
				},
				{
					"selector": "edge",
					"css": {
						"width": "mapData(width_mapper, 0, 1, 1, 10)"
					}
				},
				{
					"selector": "edge",
					"css": {
						"line-color": "mapData(color_mapper, 0, 1, yellow, green)"
					}
				}
			],
			"scratch": {},
			"zoomingEnabled": true,
			"userZoomingEnabled": true,
			"zoom": 0.9289219756347901,
			"minZoom": 1e-50,
			"maxZoom": 1e+50,
			"panningEnabled": true,
			"userPanningEnabled": true,
			"pan": {
				"x": 28.4858126405461,
				"y": 14.939713543598458
			},
			"boxSelectionEnabled": true,
			"layout": {
				"name": "grid"
			},
			"renderer": {
				"name": "canvas"
			},
			"hideEdgesOnViewport": false
		},
		"differ": {
			"elements": {
				"nodes": [
					{
						"data": {
							"id": "Akt",
							"name": "Akt"
						},
						"position": {
							"x": 68.25,
							"y": 51.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Erk",
							"name": "Erk"
						},
						"position": {
							"x": 204.75,
							"y": 51.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Mek",
							"name": "Mek"
						},
						"position": {
							"x": 68.25,
							"y": 155.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "PI3K",
							"name": "PI3K"
						},
						"position": {
							"x": 204.75,
							"y": 155.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "RTK",
							"name": "RTK"
						},
						"position": {
							"x": 68.25,
							"y": 258.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Raf",
							"name": "Raf"
						},
						"position": {
							"x": 204.75,
							"y": 258.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Tsc",
							"name": "Tsc"
						},
						"position": {
							"x": 66.833984375,
							"y": 364.3740234375
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "mTor",
							"name": "mTor"
						},
						"position": {
							"x": 204.75,
							"y": 362.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					}
				],
				"edges": [
					{
						"data": {
							"frequency": 0,
							"pearson": 0,
							"source": "PI3K",
							"target": "Akt",
							"color_mapper": 0,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e15"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": 0,
							"source": "Mek",
							"target": "Erk",
							"color_mapper": 0,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e16"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": 0,
							"source": "Raf",
							"target": "Mek",
							"color_mapper": 0,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e17"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": -1.221245327087672e-15,
							"source": "Erk",
							"target": "PI3K",
							"color_mapper": 1.221245327087672e-15,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e18"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "negative"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": 1.493249968120836e-14,
							"source": "RTK",
							"target": "PI3K",
							"color_mapper": 1.493249968120836e-14,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e19"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": -1.48769885299771e-14,
							"source": "mTor",
							"target": "PI3K",
							"color_mapper": 1.48769885299771e-14,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e20"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "negative"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": 0,
							"source": "RTK",
							"target": "RTK",
							"color_mapper": 0,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e21"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": -0.008918820531488958,
							"pearson": 0.01223129858231259,
							"source": "Akt",
							"target": "Raf",
							"color_mapper": 0.01223129858231259,
							"width_mapper": 0.008918820531488958,
							"target_arrow_shape": "circle",
							"line_style": "dashed",
							"id": "e22"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": 0.0103912859376819,
							"source": "Erk",
							"target": "Raf",
							"color_mapper": 0.0103912859376819,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e23"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0.007341342070137147,
							"pearson": 0.02180185233647791,
							"source": "PI3K",
							"target": "Raf",
							"color_mapper": 0.02180185233647791,
							"width_mapper": 0.007341342070137147,
							"target_arrow_shape": "circle",
							"line_style": "solid",
							"id": "e24"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": -0.01039128593768085,
							"source": "RTK",
							"target": "Raf",
							"color_mapper": 0.01039128593768085,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e25"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "negative"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": -1.099120794378905e-14,
							"source": "Akt",
							"target": "Tsc",
							"color_mapper": 1.099120794378905e-14,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e26"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "negative"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": -2.048361480433414e-14,
							"source": "Erk",
							"target": "Tsc",
							"color_mapper": 2.048361480433414e-14,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e27"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "negative"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": 2.048361480433414e-14,
							"source": "Erk",
							"target": "mTor",
							"color_mapper": 2.048361480433414e-14,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e28"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "positive"
					},
					{
						"data": {
							"frequency": 0,
							"pearson": -1.099120794378905e-14,
							"source": "Tsc",
							"target": "mTor",
							"color_mapper": 1.099120794378905e-14,
							"width_mapper": 0,
							"target_arrow_shape": "circle",
							"line_style": "dotted",
							"id": "e29"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": "negative"
					}
				]
			},
			"style": [
				{
					"selector": "node, edge",
					"css": {
						"text-valign": "top",
						"text-halign": "center",
						"color": "#000",
						"content": "",
						"text-outline-color": "#000",
						"text-outline-width": "0px",
						"text-outline-opacity": "1",
						"text-opacity": "1",
						"text-decoration": "none",
						"text-transform": "none",
						"font-family": "Helvetica",
						"font-style": "normal",
						"font-variant": "normal",
						"font-weight": "normal",
						"font-size": "16px",
						"min-zoomed-font-size": "0px",
						"display": "element",
						"visibility": "visible",
						"opacity": "1",
						"z-index": "0",
						"overlay-padding": "10px",
						"overlay-color": "#000",
						"overlay-opacity": "0",
						"transition-property": "none",
						"transition-duration": "0s",
						"transition-delay": "0s",
						"background-blacken": "0",
						"background-color": "#888",
						"background-opacity": "1",
						"background-image": "none",
						"background-position-x": "50%",
						"background-position-y": "50%",
						"background-repeat": "no-repeat",
						"background-fit": "none",
						"background-clip": "node",
						"pie-size": "100%",
						"pie-1-background-color": "black",
						"pie-2-background-color": "black",
						"pie-3-background-color": "black",
						"pie-4-background-color": "black",
						"pie-5-background-color": "black",
						"pie-6-background-color": "black",
						"pie-7-background-color": "black",
						"pie-8-background-color": "black",
						"pie-9-background-color": "black",
						"pie-10-background-color": "black",
						"pie-11-background-color": "black",
						"pie-12-background-color": "black",
						"pie-13-background-color": "black",
						"pie-14-background-color": "black",
						"pie-15-background-color": "black",
						"pie-16-background-color": "black",
						"pie-1-background-size": "0%",
						"pie-2-background-size": "0%",
						"pie-3-background-size": "0%",
						"pie-4-background-size": "0%",
						"pie-5-background-size": "0%",
						"pie-6-background-size": "0%",
						"pie-7-background-size": "0%",
						"pie-8-background-size": "0%",
						"pie-9-background-size": "0%",
						"pie-10-background-size": "0%",
						"pie-11-background-size": "0%",
						"pie-12-background-size": "0%",
						"pie-13-background-size": "0%",
						"pie-14-background-size": "0%",
						"pie-15-background-size": "0%",
						"pie-16-background-size": "0%",
						"border-color": "#000",
						"border-opacity": "1",
						"border-width": "0px",
						"border-style": "solid",
						"height": "30px",
						"width": "30px",
						"padding-left": "0px",
						"padding-right": "0px",
						"padding-top": "0px",
						"padding-bottom": "0px",
						"shape": "ellipse",
						"source-arrow-shape": "none",
						"target-arrow-shape": "none",
						"source-arrow-color": "#bbb",
						"target-arrow-color": "#bbb",
						"source-arrow-fill": "filled",
						"target-arrow-fill": "filled",
						"line-style": "solid",
						"line-color": "#bbb",
						"control-point-step-size": "40px",
						"control-point-weight": "0.5",
						"curve-style": "bezier"
					}
				},
				{
					"selector": "node > node",
					"css": {
						"background-opacity": "0.5",
						"height": "auto",
						"width": "auto",
						"padding-left": "10px",
						"padding-right": "10px",
						"padding-top": "10px",
						"padding-bottom": "10px",
						"shape": "rectangle"
					}
				},
				{
					"selector": "edge",
					"css": {
						"width": "1px"
					}
				},
				{
					"selector": ":active",
					"css": {
						"overlay-padding": "10px",
						"overlay-color": "black",
						"overlay-opacity": "0.25"
					}
				},
				{
					"selector": "core",
					"css": {
						"selection-box-color": "#ddd",
						"selection-box-opacity": "0.65",
						"selection-box-border-color": "#aaa",
						"selection-box-border-width": "1px",
						"panning-cursor": "grabbing",
						"active-bg-color": "black",
						"active-bg-opacity": "0.15",
						"active-bg-size": "15px",
						"outside-texture-bg-color": "#000",
						"outside-texture-bg-opacity": "0.125"
					}
				},
				{
					"selector": "node",
					"css": {
						"text-valign": "center",
						"content": "data(name)",
						"background-color": "white",
						"border-color": "black",
						"border-width": "2px",
						"height": "50px",
						"width": "50px"
					}
				},
				{
					"selector": "edge",
					"css": {
						"target-arrow-shape": "data(target_arrow_shape)",
						"target-arrow-color": "black",
						"line-style": "data(line_style)"
					}
				},
				{
					"selector": "edge",
					"css": {
						"width": "mapData(width_mapper, 0, 1, 1, 10)"
					}
				},
				{
					"selector": "edge.positive",
					"css": {
						"line-color": "mapData(color_mapper, 0, 1, yellow, green)"
					}
				},
				{
					"selector": "edge.negative",
					"css": {
						"line-color": "mapData(color_mapper, 0, 1, yellow, red)"
					}
				}
			],
			"scratch": {},
			"zoomingEnabled": true,
			"userZoomingEnabled": true,
			"zoom": 0.9289219756347901,
			"minZoom": 1e-50,
			"maxZoom": 1e+50,
			"panningEnabled": true,
			"userPanningEnabled": true,
			"pan": {
				"x": 28.4858126405461,
				"y": 14.939713543598458
			},
			"boxSelectionEnabled": true,
			"layout": {
				"name": "grid"
			},
			"renderer": {
				"name": "canvas"
			},
			"hideEdgesOnViewport": false
		},
		"compare": {
			"elements": {
				"nodes": [
					{
						"data": {
							"id": "Akt",
							"name": "Akt"
						},
						"position": {
							"x": 68.5,
							"y": 51.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Erk",
							"name": "Erk"
						},
						"position": {
							"x": 205.5,
							"y": 51.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Mek",
							"name": "Mek"
						},
						"position": {
							"x": 68.5,
							"y": 155.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "PI3K",
							"name": "PI3K"
						},
						"position": {
							"x": 205.5,
							"y": 155.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "RTK",
							"name": "RTK"
						},
						"position": {
							"x": 68.5,
							"y": 258.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Raf",
							"name": "Raf"
						},
						"position": {
							"x": 205.5,
							"y": 258.75
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "Tsc",
							"name": "Tsc"
						},
						"position": {
							"x": 68.5,
							"y": 362.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"id": "mTor",
							"name": "mTor"
						},
						"position": {
							"x": 205.5,
							"y": 362.25
						},
						"group": "nodes",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					}
				],
				"edges": [
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "PI3K",
							"target": "Akt",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e30"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "Mek",
							"target": "Erk",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e31"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "Raf",
							"target": "Mek",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e32"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.8181818181818182,
							"pearson": -0.3489016221354863,
							"source": "Erk",
							"target": "PI3K",
							"color_mapper": 0.3489016221354863,
							"width_mapper": 0.8181818181818182,
							"target_arrow_shape": "tee",
							"id": "e33"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 0.4538743983518276,
							"source": "RTK",
							"target": "PI3K",
							"color_mapper": 0.4538743983518276,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e34"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.4538743983518208,
							"source": "mTor",
							"target": "PI3K",
							"color_mapper": 0.4538743983518208,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e35"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 1,
							"source": "RTK",
							"target": "RTK",
							"color_mapper": 1,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e36"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.926829268292683,
							"pearson": -0.3442451657755377,
							"source": "Akt",
							"target": "Raf",
							"color_mapper": 0.3442451657755377,
							"width_mapper": 0.926829268292683,
							"target_arrow_shape": "tee",
							"id": "e37"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.3796634992642713,
							"source": "Erk",
							"target": "Raf",
							"color_mapper": 0.3796634992642713,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e38"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.9105691056910569,
							"pearson": 0.3102120148567476,
							"source": "PI3K",
							"target": "Raf",
							"color_mapper": 0.3102120148567476,
							"width_mapper": 0.9105691056910569,
							"target_arrow_shape": "triangle",
							"id": "e39"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": 0.3796634992642704,
							"source": "RTK",
							"target": "Raf",
							"color_mapper": 0.3796634992642704,
							"width_mapper": 1,
							"target_arrow_shape": "triangle",
							"id": "e40"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.7182335127931944,
							"source": "Akt",
							"target": "Tsc",
							"color_mapper": 0.7182335127931944,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e41"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.6666666666666666,
							"pearson": -0.3849001794597454,
							"source": "Erk",
							"target": "Tsc",
							"color_mapper": 0.3849001794597454,
							"width_mapper": 0.6666666666666666,
							"target_arrow_shape": "tee",
							"id": "e42"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 0.6666666666666666,
							"pearson": 0.3849001794597454,
							"source": "Erk",
							"target": "mTor",
							"color_mapper": 0.3849001794597454,
							"width_mapper": 0.6666666666666666,
							"target_arrow_shape": "triangle",
							"id": "e43"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					},
					{
						"data": {
							"frequency": 1,
							"pearson": -0.7182335127931944,
							"source": "Tsc",
							"target": "mTor",
							"color_mapper": 0.7182335127931944,
							"width_mapper": 1,
							"target_arrow_shape": "tee",
							"id": "e44"
						},
						"position": {},
						"group": "edges",
						"removed": false,
						"selected": false,
						"selectable": true,
						"locked": false,
						"grabbed": false,
						"grabbable": true,
						"classes": ""
					}
				]
			},
			"style": [
				{
					"selector": "node, edge",
					"css": {
						"text-valign": "top",
						"text-halign": "center",
						"color": "#000",
						"content": "",
						"text-outline-color": "#000",
						"text-outline-width": "0px",
						"text-outline-opacity": "1",
						"text-opacity": "1",
						"text-decoration": "none",
						"text-transform": "none",
						"font-family": "Helvetica",
						"font-style": "normal",
						"font-variant": "normal",
						"font-weight": "normal",
						"font-size": "16px",
						"min-zoomed-font-size": "0px",
						"display": "element",
						"visibility": "visible",
						"opacity": "1",
						"z-index": "0",
						"overlay-padding": "10px",
						"overlay-color": "#000",
						"overlay-opacity": "0",
						"transition-property": "none",
						"transition-duration": "0s",
						"transition-delay": "0s",
						"background-blacken": "0",
						"background-color": "#888",
						"background-opacity": "1",
						"background-image": "none",
						"background-position-x": "50%",
						"background-position-y": "50%",
						"background-repeat": "no-repeat",
						"background-fit": "none",
						"background-clip": "node",
						"pie-size": "100%",
						"pie-1-background-color": "black",
						"pie-2-background-color": "black",
						"pie-3-background-color": "black",
						"pie-4-background-color": "black",
						"pie-5-background-color": "black",
						"pie-6-background-color": "black",
						"pie-7-background-color": "black",
						"pie-8-background-color": "black",
						"pie-9-background-color": "black",
						"pie-10-background-color": "black",
						"pie-11-background-color": "black",
						"pie-12-background-color": "black",
						"pie-13-background-color": "black",
						"pie-14-background-color": "black",
						"pie-15-background-color": "black",
						"pie-16-background-color": "black",
						"pie-1-background-size": "0%",
						"pie-2-background-size": "0%",
						"pie-3-background-size": "0%",
						"pie-4-background-size": "0%",
						"pie-5-background-size": "0%",
						"pie-6-background-size": "0%",
						"pie-7-background-size": "0%",
						"pie-8-background-size": "0%",
						"pie-9-background-size": "0%",
						"pie-10-background-size": "0%",
						"pie-11-background-size": "0%",
						"pie-12-background-size": "0%",
						"pie-13-background-size": "0%",
						"pie-14-background-size": "0%",
						"pie-15-background-size": "0%",
						"pie-16-background-size": "0%",
						"border-color": "#000",
						"border-opacity": "1",
						"border-width": "0px",
						"border-style": "solid",
						"height": "30px",
						"width": "30px",
						"padding-left": "0px",
						"padding-right": "0px",
						"padding-top": "0px",
						"padding-bottom": "0px",
						"shape": "ellipse",
						"source-arrow-shape": "none",
						"target-arrow-shape": "none",
						"source-arrow-color": "#bbb",
						"target-arrow-color": "#bbb",
						"source-arrow-fill": "filled",
						"target-arrow-fill": "filled",
						"line-style": "solid",
						"line-color": "#bbb",
						"control-point-step-size": "40px",
						"control-point-weight": "0.5",
						"curve-style": "bezier"
					}
				},
				{
					"selector": "node > node",
					"css": {
						"background-opacity": "0.5",
						"height": "auto",
						"width": "auto",
						"padding-left": "10px",
						"padding-right": "10px",
						"padding-top": "10px",
						"padding-bottom": "10px",
						"shape": "rectangle"
					}
				},
				{
					"selector": "edge",
					"css": {
						"width": "1px"
					}
				},
				{
					"selector": ":active",
					"css": {
						"overlay-padding": "10px",
						"overlay-color": "black",
						"overlay-opacity": "0.25"
					}
				},
				{
					"selector": "core",
					"css": {
						"selection-box-color": "#ddd",
						"selection-box-opacity": "0.65",
						"selection-box-border-color": "#aaa",
						"selection-box-border-width": "1px",
						"panning-cursor": "grabbing",
						"active-bg-color": "black",
						"active-bg-opacity": "0.15",
						"active-bg-size": "15px",
						"outside-texture-bg-color": "#000",
						"outside-texture-bg-opacity": "0.125"
					}
				},
				{
					"selector": "node",
					"css": {
						"text-valign": "center",
						"content": "data(name)",
						"background-color": "white",
						"border-color": "black",
						"border-width": "2px",
						"height": "50px",
						"width": "50px"
					}
				},
				{
					"selector": "edge",
					"css": {
						"target-arrow-shape": "data(target_arrow_shape)",
						"target-arrow-color": "black",
						"line-style": "data(line_style)"
					}
				},
				{
					"selector": "edge",
					"css": {
						"width": "mapData(width_mapper, 0, 1, 1, 10)"
					}
				},
				{
					"selector": "edge",
					"css": {
						"line-color": "mapData(color_mapper, 0, 1, yellow, red)"
					}
				}
			],
			"scratch": {},
			"zoomingEnabled": true,
			"userZoomingEnabled": true,
			"zoom": 0.9289219756347901,
			"minZoom": 1e-50,
			"maxZoom": 1e+50,
			"panningEnabled": true,
			"userPanningEnabled": true,
			"pan": {
				"x": 28.4858126405461,
				"y": 14.939713543598458
			},
			"boxSelectionEnabled": true,
			"layout": {
				"name": "grid"
			},
			"renderer": {
				"name": "canvas"
			},
			"hideEdgesOnViewport": false
		}
	}
};