#! /usr/bin/env python

import rospy
import rosbag
import os,sys
import glob

def readBagFile(filename):
	output = ''
	bag = rosbag.Bag(filename)
	for topic, msg, t in bag.read_messages('/vslam/pose'):
		
		x = str(msg.pose.pose.position.x)
		y = str(msg.pose.pose.position.y)
		z = str(msg.pose.pose.position.z)
		
		qx = str(msg.pose.pose.orientation.x)
		qy = str(msg.pose.pose.orientation.y)
		qz = str(msg.pose.pose.orientation.z)
		qw = str(msg.pose.pose.orientation.w)

		ts = str(msg.header.stamp.secs) + '.' + str(msg.header.stamp.nsecs)
		
		
		output += x + '\t' + y + '\t' + z + '\t' + qx + '\t' + qy + '\t' + qz + '\t' + qw + '\t' + ts + '\n'
 	bag.close()
	return output

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
		output = readBagFile(b)
		outputFilename = b.rstrip('.bag') + '.txt'
		writeToFile(outputFilename,output)
		print 'Output written to ' + outputFilename + '.'
	print 'Done.'
               

if __name__ == '__main__':
   sys.exit( main() )
