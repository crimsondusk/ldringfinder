/*
 *  Copyright (C) 2013 Santeri Piippo
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui.h"
#include "algorithm.h"
#include "ui_main.h"
#include "version.h"

static const QString g_bestBlurb = MainWindow::tr ("<span style=\"color: #280\"> [ best ]</span>");

MainWindow::MainWindow (QWidget* parent, Qt::WindowFlags flags) : QMainWindow (parent, flags)
{	m_ui = new Ui_MainWindow;
	m_ui->setupUi (this);

	connect (m_ui->m_go, SIGNAL (clicked (bool)), this, SLOT (go()));
	connect (m_ui->m_min, SIGNAL (valueChanged (double)), this, SLOT (inputChanged()));
	connect (m_ui->m_max, SIGNAL (valueChanged (double)), this, SLOT (inputChanged()));

	setWindowTitle (QString (APPNAME " %1.%2").arg (VERSION_MAJOR).arg (VERSION_MINOR));
}

MainWindow::~MainWindow()
{	delete m_ui;
}

void MainWindow::inputChanged()
{	m_ui->m_go->setEnabled (m_ui->m_min->value() > 0 && m_ui->m_max->value() > 0);
}

void MainWindow::writeSolution (int num, const RingFinder::Solution& sol, QString& text)
{	text += tr ("<h3>Solution %1%2</h3>")
		.arg (num + 1)
		.arg (&sol == g_RingFinder.bestSolution() ? g_bestBlurb : QString());
	text += "<ul>";

	for (const RingFinder::Component& cmp : sol.components())
		text += tr ("<li>Ring %1, scaled by %2 (%3 -> %4)</li>")
			.arg (cmp.num)
			.arg (cmp.scale)
			.arg (cmp.num * cmp.scale)
			.arg ((cmp.num + 1) * cmp.scale);

	text += "</ul>";
}

void MainWindow::go()
{	QString text;
	double r0 = m_ui->m_min->value();
	double r1 = m_ui->m_max->value();

	if (g_RingFinder (r0, r1))
	{	if (m_ui->m_showAll->isChecked())
		{	for (int i = 0; i < g_RingFinder.allSolutions().size(); ++i)
				writeSolution (i, g_RingFinder.allSolutions()[i], text);
		}
		else
			writeSolution (0, *g_RingFinder.bestSolution(), text);
	}
	else
		text = tr ("Failed to find solutions for radii %1 and %2.").arg (r0).arg (r1);

	m_ui->m_results->setHtml (text);
}