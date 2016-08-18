#!/usr/bin/python
#
# Python Version Header Generator Script
#
# Uses Semantic Version (http://semver.org/) based git tags to generate header files
# containing useful version information
# To create an appropriate tag, use `git tag -a "vX.X.X" -m "message here"
# For more information, see: 
# http://electronpowered.org/2015/01/20/using-git-tags-to-track-software-versions/
 
import argparse;
import subprocess;
import re;
import sys;
 
#Parse input argument
parser = argparse.ArgumentParser(description='Generate `git describe` based version header file');
parser.add_argument('module', type=str, help='module name');
parser.add_argument('output', type=str, help='output file name');
 
args = parser.parse_args();
output = args.output;
module = args.module;
 
#Load git description
p = subprocess.Popen('git describe --dirty', stdout=subprocess.PIPE, shell=True)
version = p.communicate()[0]
versionString = version.replace('\n', '').replace('\r', '');
 
# Extract version numbers
regex = re.match('v([0-9]+).([0-9]+).([0-9]+)', versionString);
if (not regex):
    print 'Version Generator Error: Invalid version tag (see http://semver.org/)';
    sys.exit(-1);
 
major = regex.group(1);
minor = regex.group(2);
patch = regex.group(3);
 
#Generate new file string
newString = (
    '#ifndef ' + module + '_VERSION_H\r\n' + 
    '#define ' + module + '_VERSION_H\r\n' +
    '#define ' + module + '_VERSION_STRING \"' + versionString + '\"\r\n' +
    '#define ' + module + '_VERSION_MAJOR ' + major + '\r\n' +
    '#define ' + module + '_VERSION_MINOR ' + minor + '\r\n' +
    '#define ' + module + '_VERSION_PATCH ' + patch + '\r\n' +
    '#endif\r\n'
    )
 
#Load existing file string
try:
    f = open(output, 'r');
    f.seek(0);
    fileString = f.read();
    f.close();
except IOError, e:
    fileString = "";
 
#Check for differences and update if required
if(fileString != newString):
    print 'Version Generator: updating header file';
    f = open(output, 'w+');
    f.seek(0);
    f.write(newString);
    f.truncate();
    f.close();