#include "MainWindow.h"
#include <kotono_interface/widgets.h>

WWidget* WMainWindow::Build()
{
    return new WCenter({
        .child = new WStack({
            .children = {
                new WColor({
                    .color = KtColor::Red().WithValue(0.1f),
                }),
                new WPadding({
                    .padding = WPadding::Padding::All(8.0f),
                    .child = new WColumn({
                        .spacing = 10.0f,
                        .children = {
                            new WConstraint({
                                .axis = WConstraint::Axis::Vertical,
                                .size = 32.0f,
                                .child = new WRow({
                                    .spacing = 5.0f,
                                    .children = {
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                    },
                                }),
                            }),
                            
                            new WConstraint({
                                .axis = WConstraint::Axis::Vertical,
                                .size = 32.0f,
                                .child = new WRow({
                                    .spacing = 5.0f,
                                    .children = {
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                        new WColor({ .color = KtColor::White(), }),
                                    },
                                }),
                            }),

                            new WRow({
                                .spacing = 10.0f,
                                .children = {
                                    new WColor({ .color = KtColor::White(), }),
                                    new WColor({ .color = KtColor::White(), }),
                                    new WColor({ .color = KtColor::White(), }),
                                },
                            }),
                        },
                    }),
                }),
            },
        }),
    });
}
