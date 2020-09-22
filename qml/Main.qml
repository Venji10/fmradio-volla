/*
 * Copyright (C) 2020 venji10 <bennisteinir@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import QtQuick 2.7
import Ubuntu.Components 1.3
//import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0

import MediatekRadio 1.0

MainView {
	id: root
	objectName: 'mainView'
	applicationName: 'fmradio.venji10'
	automaticOrientation: false

	width: units.gu(45)
	height: units.gu(75)

	Page {
		anchors.fill: parent

		header: PageHeader {
			id: header
			title: i18n.tr('FM Radio')
		}

		GridLayout {
			columns: 3

			anchors {
				top: header.bottom
				margins: units.gu(2)
				left: parent.left
				right: parent.right
				bottom: parent.bottom
			}

			Item {
				height: units.gu(30)
			}

			Item {
				height: units.gu(30)
			}

			Item {
				height: units.gu(30)
			}

			Button {
				text: i18n.tr('\<')
				onClicked: {
					MediatekRadio.seekDown()
					frequency.text = MediatekRadio.getFrequency() / 100
				}
			}

			Label {
				id: frequency
				font.pixelSize: units.gu(10)
				text: MediatekRadio.getFrequency() / 100
			}

			Button {
				text: i18n.tr('\>')
				onClicked: {
					MediatekRadio.seekUp()
					frequency.text = MediatekRadio.getFrequency() / 100
				}
			}

			Item {
				Layout.fillHeight: true
			}

			Button {
				id: startStopButton
				Layout.alignment: Qt.AlignHCenter
				text: i18n.tr('Start radio')
				onClicked: if(!MediatekRadio.isRadioRunning()) {
						text = MediatekRadio.startRadio(MediatekRadio.getFrequency())
					} else {
						text = MediatekRadio.stopRadio()
					}
			}

/*			Item {
				Layout.fillHeight: true
			}
*/

		}

		BottomEdge {
			id: bottomEdge
			width: units.gu(45)
			height: units.gu(40.5)
//			preloadContent: true

			contentComponent: TextInput {
				id: frequencyInput
				font.pixelSize: units.gu(5)
				inputMask: "990.9;_"
				inputMethodHints: Qt.ImhFormattedNumbersOnly
				text: MediatekRadio.getFrequency() / 100
				onEditingFinished: {
					MediatekRadio.tune(frequencyInput.text * 100)
					frequency.text = MediatekRadio.getFrequency() / 100
					bottomEdge.collapse()
				}

				Connections {
					target: bottomEdge
					onCommitCompleted: {
						focus = true
					}
				}

				Connections {
					target: bottomEdge
					onCollapseCompleted: {
						focus = false
					}
				}


			}

		}

	}
}
