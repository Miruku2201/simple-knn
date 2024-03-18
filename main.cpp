#include"kNN.hpp"

int main() {
  //   OList <int> list;
	 //list.push_back(1);
	 //list.push_back(2);
	 //list.push_back(3);
	 //list.push_back(4);
	 //list.push_back(5);
	 //list.push_back(6);
	 //list.reverse();
	 //list.print();
	 // std::cout << list.get(3);


	//Dataset dataset;
	//dataset.loadFromCSV("mnist.csv");
	////dataset.drop(1, 0, "1x1");
	////dataset.printHead();

	//Dataset dataExtracted = dataset.extract(1, -1, 0, 3);
	//int r = 0;
	//int c = 0;
	//dataExtracted.getShape(r, c);
	//std::cout << r << " " << c << std::endl;
	//dataExtracted.getShape(r, c);
	//dataExtracted.printHead(199,5);

	//Dataset dataset;
	//dataset.loadFromCSV("mnist.csv");
	//dataset.printHead();
	//dataset.printTail();
	//int nRows, nCols;
	//dataset.getShape(nRows, nCols);
	//cout << "Shape: " << nRows << "x" << nCols << endl;

	//kNN knn;
	//Dataset X_train, X_test, y_train, y_test;
	//Dataset feature = dataset.extract(0, -1, 1, -1);
	//Dataset label = dataset.extract(0, -1, 0, 0);
	//train_test_split(feature, label, 0.9, X_train, X_test, y_train, y_test);


	//feature.printHead();
	//knn.fit(X_train, y_train);
	//Dataset y_pred = knn.predict(X_test);
	//std::cout << "Accuracy: " << knn.score(y_test, y_pred);
	//return 0;

	Dataset dataset;
	dataset.loadFromCSV("mnist.csv");
	dataset.printHead(10, 10);
	cout << endl;
	dataset.printTail(10, 10);
	cout << endl;
	int nRows, nCols;
	dataset.getShape(nRows, nCols);
	cout << "Shape: " << nRows << "x" << nCols << endl;
	return 0;
}