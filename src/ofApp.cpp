#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(640,480);

		colorImg.allocate(640,480);
		grayImg.allocate( 640,480);

}

//--------------------------------------------------------------
void ofApp::update(){

	ofBackground(100,100,100);

	bool bNewFrame = false;

    vidGrabber.update();
	bNewFrame = vidGrabber.isFrameNew();


	if (bNewFrame){
         colorImg.setFromPixels(vidGrabber.getPixels(), 640,480);
		 grayImg = colorImg;
	}
	
	for (int i = 0; i < templates.size(); i++)
	{
		
		cvMatchTemplate(grayImg.getCvImage(), templates[i].getCvImage(), results[i], CV_TM_CCOEFF); // &results[i] gets the point to results[i] rather than results[i] its self, maybe....
		cvMinMaxLoc(results[i], &markers[i].minVal, &markers[i].maxVal, &markers[i].minLoc, &markers[i].maxLoc, 0);
		
	}


}

void ofApp::loadImagesButtonPressed(){
	
	//ofFileDialogResult fileResult = ofSystemLoadDialog(

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xffffff);
	grayImg.draw(20,20);

	int heightOffset = 20;

	for (int i = 0; i < templates.size(); i++)
	{
		
		templates[i].draw((20+grayImg.getWidth()+20), heightOffset);
		//ofxCvGrayscaleImage newImage;
		//ofPixels myNewPixels;
		//unsigned char myPixels = *results[i]->imageData;
		//myNewPixels.setFromPixels(&myPixels, results[i]->width, results[i]->height, 1);
		//newImage.setFromPixels(&myPixels, results[i]->height, results[i]->width);



		//ofNoFill();
		//ofPoint centerOfMarker = ofPoint(markers[i].maxLoc.x+(templates[i].getWidth/2),markers[i].maxLoc.y+(templates[i].getHeight/2));
		//ofCircle(ofPoint(centerOfMarker.x+20, centerOfMarker.y+20) , 3);

		//ofLine(ofPoint(markers[i].maxLoc.x+20,markers[i].maxLoc.y+20), ofPoint((20+grayImg.getWidth()+20), heightOffset));
		//heightOffset += templates[i].getHeight() + 20;

	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
	ofFile newFile;
	//Check if the file exists
	if (!newFile.doesFileExist(dragInfo.files[0])) {
		//if it doesnt, let someone know
		ofSystemAlertDialog("Could not load image with path : \n" + ofToString(dragInfo.files[0]) + "\nMake sure its on the same drive as the executable \nSheesh....");
	}
	//If it does, do some cool stuff!
	else {
		//always try! (incase something is bad, like a text file is dropped in or some such)
		try {
			//Create a new ofImage and load the file into it
			ofImage newTemplateImage;
			newTemplateImage.loadImage(dragInfo.files[0]);

			if (!newTemplateImage.bAllocated())
			{
				throw 10;
			}

		
			//Images for the newly loaded template and its cvMatchTemplate result
			ofxCvColorImage		newTemplateCvColorImage;
			ofxCvGrayscaleImage newTemplateCvGrayImage;
			ofxCvGrayscaleImage newResultCvGrayImage;
			//and a new IPLimage (an oepnCV native type) to hold the cvMatchTemplate result
			//IplImage *newResultCvGrayImage = cvCreateImage(cvSize(camWidth – subjectImg.width + 1, camHeight – subjectImg.height + 1), 32, 1); ;

			//Allocate the template images with the size of the loaded tempalte
			newTemplateCvColorImage.allocate(newTemplateImage.getWidth(), newTemplateImage.getHeight());
			newTemplateCvGrayImage.allocate( newTemplateImage.getWidth(), newTemplateImage.getHeight());

			//Pop the camera pixels into the color image and convert it to greyscale
			newTemplateCvColorImage.setFromPixels(newTemplateImage.getPixels(), newTemplateImage.getWidth(), newTemplateImage.getHeight());
			newTemplateCvGrayImage = newTemplateCvColorImage;

			//and push the image into our list
			templates.push_back(newTemplateCvGrayImage);

			//Now for the result images, whcih we can also initialise on tempalte load

			//Allocatge the result image with a size relative to the camera and template images
			int resultWidth	= grayImg.getWidth() -	newTemplateImage.getWidth() + 1;
			int resultHeight	= grayImg.getHeight() - newTemplateImage.getHeight() + 1;

			if (resultWidth >  0 && resultHeight > 0)
			{
				IplImage *newIplImageResult = cvCreateImage(cvSize(resultWidth, resultHeight), 32, 1);
				//and do the same for the result image
				results.push_back(newIplImageResult);

				Marker newMarker;
				markers.push_back(newMarker);
			}
			else
			{
				ofSystemAlertDialog("couldnt allocate result image. Is the tempalte too big perhaps?");
				throw 10;
			}







		}
		catch (int e)
		{
			ofSystemAlertDialog("Error : " + ofToString(e) + " Could not load image with path : \n" + ofToString(dragInfo.files[0]) + "\nFound the file but something went wrong in the loading process");
		}


				
		
	}





}
