#! /usr/bin/env python

import rospy
import rosbag
import os,sys
import glob

def readPoseStampedWithCovariance(filename,topic):
	output = ''
	try:
		bag = rosbag.Bag(filename)

		for topic, msg, t in bag.read_messages(topic):
			x = str(msg.pose.pose.position.x)
			y = str(msg.pose.pose.position.y)
			z = str(msg.pose.pose.position.z)
	
			qx = str(msg.pose.pose.orientation.x)
			qy = str(msg.pose.pose.orientation.y)
			qz = str(msg.pose.pose.orientation.z)
			qw = str(msg.pose.pose.orientation.w)
		
			ts = str(t.secs) + str(t.nsecs/1000000000.0).lstrip('0')
		
			output += x + '\t' + y + '\t' + z + '\t' + qx + '\t' + qy + '\t' + qz + '\t' + qw + '\t' + ts + '\n'

	 	bag.close()

		return output

	except rosbag.bag.ROSBagUnindexedException:
		print "ERROR: Unindexed bag: " + filename

def readPoseStamped(filename,topic):
	output = ''
	try:
		bag = rosbag.Bag(filename)

		for topic, msg, t in bag.read_messages(topic):
			x = str(msg.pose.position.x)
			y = str(msg.pose.position.y)
			z = str(msg.pose.position.z)
		
			qx = str(msg.pose.orientation.x)
			qy = str(msg.pose.orientation.y)
			qz = str(msg.pose.orientation.z)
			qw = str(msg.pose.orientation.w)

			ts = str(t.secs) + str(t.nsecs/1000000000.0).lstrip('0')
				
			output += x + '\t' + y + '\t' + z + '\t' + qx + '\t' + qy + '\t' + qz + '\t' + qw + '\t' + ts + '\n'
	
	 	bag.close()
		return output

	except rosbag.bag.ROSBagUnindexedException:
		print "ERROR: Unindexed bag: " + filename

def gatherBagFiles():
	files = glob.glob('*.bag')
	return files	

def writeToFile(filename, string):
	f = open(filename,'w');
	f.write(string);
	f.close()

def main():
	bagFiles = gatherBagFiles()
	for b in bagFiles:
		print 'Processing ' + b + ' ...'
		output = readPoseStampedWithCovariance(b,'/vslam/pose')
		if output is not None:
			outputFilename = b.rstrip('.bag') + '_ptam.txt'
			writeToFile(outputFilename,output)
			print 'Output written to ' + outputFilename
	
		output = readPoseStamped(b,'ardrone_to')
		if output is not None:
			outputFilename = b.rstrip('.bag') + '_drone.txt'
			print 'Output written to ' + outputFilename
			writeToFile(outputFilename,output)

		output = readPoseStamped(b,'dvs_to')
		if output is not None:
			outputFilename = b.rstrip('.bag') + '_dvs.txt'
			print 'Output written to ' + outputFilename
			writeToFile(outputFilename,output)
	print 'Done.'
               

if __name__ == '__main__':
   sys.exit( main() )
