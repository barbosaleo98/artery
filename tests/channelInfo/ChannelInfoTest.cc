/***************************************************************************
                          CHannelInfoTest.cc  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : (C) 2007 by wessel
    email                : wessel@tkn.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <omnetpp.h>
#include <ChannelInfo.h>
#include <asserts.h>

/**
 * Unit test for isInRectangle method of class Coord
 *
 * - test with one AirFrame
 * - test with removed AirFrame
 */
void testIntersections() {
	
	ChannelInfo testChannel;

	//test with one AirFrame
	AirFrame* frame1 = new AirFrame();
	frame1->setDuration(2.0);
	
	testChannel.addAirFrame(frame1, 1.0);
	
	ChannelInfo::AirFrameVector v;
	testChannel.getAirFrames(0.0, 0.9, v);	
	assertTrue("No intersecting AirFrames before single AirFrame.", v.empty());
	
	v.clear();
	testChannel.getAirFrames(3.1, 3.9, v);	
	assertTrue("No intersecting AirFrames after single AirFrame.", v.empty());
	
	v.clear();
	testChannel.getAirFrames(0.5, 1.5, v);	
	assertFalse("Cut with start should intersect.", v.empty());
	assertEqual("Cut with start should return single AirFrame.", frame1, v.front());
	
	v.clear();
	testChannel.getAirFrames(2.5, 3.5, v);	
	assertFalse("Cut with end should intersect.", v.empty());
	assertEqual("Cut with end should return single AirFrame.", frame1, v.front());
	
	v.clear();
	testChannel.getAirFrames(1.5, 2.5, v);	
	assertFalse("Interval total in AirFrame duration should intersect.", v.empty());
	assertEqual("Interval total in AirFrame duration should return single AirFrame.", frame1, v.front());
	
	v.clear();
	testChannel.getAirFrames(0.5, 3.5, v);	
	assertFalse("AirFrame total in interval duration should intersect.", v.empty());
	assertEqual("AirFrame total in interval duration should return single AirFrame.", frame1, v.front());
	
	v.clear();
	testChannel.getAirFrames(3.0, 3.9, v);	
	assertFalse("Upper border should count as intersect.", v.empty());
	assertEqual("Upper border intersection should return single AirFrame.", frame1, v.front());
	
	v.clear();
	testChannel.getAirFrames(0.0, 1.0, v);	
	assertFalse("Lower border should count as intersect.", v.empty());
	assertEqual("Lower border intersection should return single AirFrame.", frame1, v.front());
	
	
	//add another AirFrame
	AirFrame* frame2 = new AirFrame();
	frame2->setDuration(1.0);
	testChannel.addAirFrame(frame2, 2.5);
	
	v.clear();
	testChannel.getAirFrames(0.5, 2.44, v);	
	assertEqual("Interval before second AirFrame should return only first.", 1, v.size());
	assertEqual("Interval before second AirFrame should return the first AirFrame.", frame1, v.front());
	
	v.clear();
	testChannel.getAirFrames(3.05, 4.0, v);	
	assertEqual("Interval after first AirFrame should return only second.", 1, v.size());
	assertEqual("Interval after first AirFrame should return the second AirFrame.", frame2, v.front());
	
	v.clear();
	testChannel.getAirFrames(2.44, 2.55, v);	
	assertEqual("Interval inside both AirFrames should return both.", 2, v.size());
	bool bothReturned =    (v.front() == frame1 && v.back() == frame2) 
						|| (v.front() == frame2 && v.back() == frame1);
	assertTrue("Interval inside both AirFrame should return both.", bothReturned);
	
	//remove first one
	testChannel.removeAirFrame(frame1);
	
	v.clear();
	testChannel.getAirFrames(2.51, 2.9, v);	
	assertEqual("Interval inside both AirFrame should return also the deleted.", 2, v.size());
	bothReturned =    (v.front() == frame1 && v.back() == frame2) 
							|| (v.front() == frame2 && v.back() == frame1);
	assertTrue("Interval inside both AirFrame should return also the deleted.", bothReturned);
	
	//add another AirFrame which intersects with second but not with first
	
	AirFrame* frame3 = new AirFrame();
	frame3->setDuration(1.5);
	testChannel.addAirFrame(frame3, 3.5);
	
	v.clear();
	testChannel.getAirFrames(2.51, 3.5, v);	
	assertEqual("Interval inside all AirFrame should return all.", 3, v.size());
	
	//remove second AirFrame
	testChannel.removeAirFrame(frame2);
	
	v.clear();
	testChannel.getAirFrames(1.51, 2.0, v);	
	assertTrue("Interval before second frame should be empty (first one is deleted).", v.empty());
	
	v.clear();
	testChannel.getAirFrames(3.5, 3.6, v);	
	assertEqual("Interval inside second and third AirFrame should return also the second (deleted).", 2, v.size());
	bothReturned =    (v.front() == frame2 && v.back() == frame3) 
							|| (v.front() == frame3 && v.back() == frame2);
	assertTrue("Interval inside both AirFrame should return also the deleted.", bothReturned);
	
	//remove third AirFrame
	testChannel.removeAirFrame(frame3);
	
	v.clear();
	testChannel.getAirFrames(0.0, 10.0, v);	
	assertTrue("There shouldn't be anymore AirFrames.", v.empty());
	
	
	//add two airframes with same start and end
	AirFrame* frame4 = new AirFrame();
	frame4->setDuration(1.0);
	AirFrame* frame4b = new AirFrame();
	frame4b->setDuration(1.0);
	testChannel.addAirFrame(frame4b, 15.0);	
	testChannel.addAirFrame(frame4, 15.0);
	
	v.clear();
	testChannel.getAirFrames(14.5, 15.0, v);	
	assertEqual("Check for simultaneus airframes", 2, v.size());
	bothReturned =    (v.front() == frame4b && v.back() == frame4) 
								|| (v.front() == frame4 && v.back() == frame4b);
	assertTrue("Check for simultaneus airframes.", bothReturned);
	
	
	//remove one of them
	testChannel.removeAirFrame(frame4);
	
	v.clear();
	testChannel.getAirFrames(14.5, 15.0, v);	
	assertEqual("Check for simultaneus airframes after remove of one.", 2, v.size());
	bothReturned =    (v.front() == frame4b && v.back() == frame4) 
								|| (v.front() == frame4 && v.back() == frame4b);
	assertTrue("Check for simultaneus airframes after remove of one.", bothReturned);
	
	//add another airframe which starts at the end of the previous ones.
	AirFrame* frame5 = new AirFrame();
	frame5->setDuration(2.0);	
	testChannel.addAirFrame(frame5, 16.0);
	
	v.clear();
	testChannel.getAirFrames(16.0, 17.0, v);	
	assertEqual("Aiframes with same start and end are intersecting.", 3, v.size());
	
	//remove the second of the simulateus AirFrames
	testChannel.removeAirFrame(frame4b);
	
	v.clear();
	testChannel.getAirFrames(16.0, 17.0, v);	
	assertEqual("Should intersect still with both removed simultaneus AirFrames.", 3, v.size());
	
	v.clear();
	testChannel.getAirFrames(16.1, 17.0, v);	
	assertEqual("Interval after simultaneus should return only third AirFrame.", 1, v.size());
	assertEqual("Interval after simultaneus should return only third AirFrame.", frame5, v.front());
	
	//create another AirFrame with same start as previous but later end
	AirFrame* frame6 = new AirFrame();
	frame6->setDuration(3.0);
	testChannel.addAirFrame(frame6, 16.0);
	
	v.clear();
	testChannel.getAirFrames(16.1, 16.1, v);	
	assertEqual("Interval at start of both AirFrames should return both.", 2, v.size());
	bothReturned =    (v.front() == frame5 && v.back() == frame6) 
								|| (v.front() == frame6 && v.back() == frame5);
	assertTrue("Interval at start of both AirFrames should return both.", bothReturned);
	
	v.clear();
	testChannel.getAirFrames(18.1, 19.0, v);	
	assertEqual("Interval after shorter AirFrame shouldn't return the shorter.", 1, v.size());
	assertEqual("Interval after shorter AirFrame shouldn't return the shorter.", frame6, v.front());
	
	//remove shorter AirFrame with same start
	testChannel.removeAirFrame(frame5);
	
	v.clear();
	testChannel.getAirFrames(16.1, 16.1, v);	
	assertEqual("Nothing should have changed after deletion of shorter AirFrame.", 2, v.size());
	bothReturned =    (v.front() == frame5 && v.back() == frame6) 
								|| (v.front() == frame6 && v.back() == frame5);
	assertTrue("Nothing should have changed after deletion of shorter AirFrame.", bothReturned);
	
	v.clear();
	testChannel.getAirFrames(18.1, 19.0, v);	
	assertEqual("Nothing should have changed after deletion of shorter AirFrame.", 1, v.size());
	assertEqual("Nothing should have changed after deletion of shorter AirFrame.", frame6, v.front());
	
	//add another one with same end as previous but later start
	AirFrame* frame7 = new AirFrame();
	frame7->setDuration(0.5);
	testChannel.addAirFrame(frame7, 18.5);
	
	v.clear();
	testChannel.getAirFrames(16.1, 16.1, v);	
	assertEqual("Interval before newly added should not return newly added.", 2, v.size());
	bothReturned =    (v.front() == frame5 && v.back() == frame6) 
								|| (v.front() == frame6 && v.back() == frame5);
	assertTrue("Interval before newly added should not return newly added.", bothReturned);
	
	v.clear();
	testChannel.getAirFrames(18.1, 18.4, v);	
	assertEqual("Interval before newly added should not return newly added.", 1, v.size());
	assertEqual("Interval before newly added should not return newly added.", frame6, v.front());
	
	v.clear();
	testChannel.getAirFrames(18.5, 18.5, v);	
	assertEqual("Newly added should be returned together with the other one.", 2, v.size());
	bothReturned =    (v.front() == frame6 && v.back() == frame7) 
								|| (v.front() == frame7 && v.back() == frame6);
	assertTrue("Newly added should be returned together with the other one.", bothReturned);
	
	v.clear();
	testChannel.getAirFrames(14.5, 15.0, v);	
	assertEqual("Our simulatneus AirFrames should be still there", 2, v.size());
	bothReturned =    (v.front() == frame4b && v.back() == frame4) 
								|| (v.front() == frame4 && v.back() == frame4b);
	assertTrue("Our simulatneus AirFrames should be still there.", bothReturned);
	
	//remove the only AirFrame still intersecting with the simultaneus ones and the shorter version
	testChannel.removeAirFrame(frame6);
	
	v.clear();
	testChannel.getAirFrames(14.5, 15.0, v);	
	assertEqual("Simultaneus AirFrames should be deleted now.", 0, v.size());
	
	v.clear();
	testChannel.getAirFrames(16.0, 16.0, v);	
	assertEqual("Only longer AirFrame should be still there.", 1, v.size());
	assertEqual("Only longer AirFrame should be still there.", frame6, v.front());
	
	v.clear();
	testChannel.getAirFrames(18.5, 18.5, v);	
	assertEqual("Last mans standing: last added and the long AirFrame.", 2, v.size());
	bothReturned =    (v.front() == frame6 && v.back() == frame7) 
								|| (v.front() == frame7 && v.back() == frame6);
	assertTrue("Last mans standing: last added and the long AirFrame.", bothReturned);
	
	//remove last AirFrame
	testChannel.removeAirFrame(frame7);
	v.clear();
	testChannel.getAirFrames(18.5, 18.5, v);	
	assertEqual("Should be empty now..", 0, v.size());
}

int main() {
    testIntersections();
}

