#include "kNN.hpp"

 Dataset::Dataset(const Dataset &other)
 {
     data = new OList<List<int> *>();
     column_name = new OList<std::string>();
     for (int i = 0; i < other.data->length(); i++)
     {
         List<int> *row = new OList<int>();
         List<int> *otherRow = other.data->get(i);
         for (int j = 0; j < otherRow->length(); j++)
         {
             row->push_back(otherRow->get(j));
         }
         data->push_back(row);
     }
     for (int i = 0; i < other.column_name->length(); i++) {
         column_name->push_back(other.column_name->get(i));
     }
 }

 Dataset &Dataset::operator=(const Dataset &other)
 {
     if (this != &other)
     {
         data = new OList<List<int> *>();
         for (int i = 0; i < other.data->length(); i++)
         {
             List<int> *row = new OList<int>();
             List<int> *otherRow = other.data->get(i);
             for (int j = 0; j < otherRow->length(); j++)
             {
                 row->push_back(otherRow->get(j));
             }
             data->push_back(row);
             
         }
         for (int i = 0; i < other.column_name->length(); i++) {
             column_name->push_back(other.column_name->get(i));
         }
     }
     return *this;
 }

bool Dataset::loadFromCSV(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    //// Save the column name.
    std::string line = ""; 
    getline(file, line);
    std::stringstream ss(line);
    std::string cell = "";
    while (getline(ss, cell, ',')) {
        column_name->push_back(cell);
    }

    //// Save the label and label's pixel.
    while (getline(file, line))
    {
        List<int>* row = new OList<int>();
        std::stringstream ss(line);
        while (getline(ss, cell, ','))
        {
            row->push_back(std::stoi(cell));
        }
        data->push_back(row);
    }
    file.close();
    return true;
}

void Dataset::printHead(int nRows, int nCols) const{
    if (nRows < 0 || nCols < 0) { return; }

    /*
        width(ncols): number of pixel + label
        height(nrows): number of picture
    */
    int width = column_name->length();
    int height = data->length();

    nCols = (nCols > width) ? width : nCols;
    nRows = (nRows > height) ? height : nRows;

    // Print the column name
    for (auto i = 0; i < nCols; i++) {
        std::cout << column_name->get(i);
        if (i < nCols-1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
    
    // Print the data 
    for (auto i = 0; i < nRows; i++) {
        List<int>* row = data->get(i);
        for (auto j = 0; j < nCols; j++) {
            std::cout << row->get(j);
            if (j < nCols - 1) {
                std::cout << " ";
            }
        }
        if (i != nRows - 1) {
            std::cout << "\n";
        }
    }
}

void Dataset::printTail(int nRows, int nCols) const{
    if (nRows < 0 || nCols < 0) { return; }

    /*
        width(ncols): number of pixel + label
        height(nrows): number of picture 
    */
    int width = column_name->length();
    int height = data->length();

    int start_column = (width - nCols < 0) ? 0: width - nCols;
    int start_row = (height - nRows < 0) ? 0 : height - nRows;
    
    // Print the column name
    for (auto i = start_column; i < width; i++) {
        std::cout << column_name->get(i);
        if (i < width - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";

    // Print the data
    for (auto i = start_row; i < height; i++) {
        List<int>* row = data->get(i);
        for (auto j = start_column; j < width; j++) {
            std::cout << row->get(j);
            if (j < width - 1) {
                std::cout << " ";
            }
        }
        if (i != height - 1) {
            std::cout << "\n";
        }
    }
}

void Dataset::getShape(int& nRows, int& nCols) const {
    /*
         number of columns: number of pixel + label
         number of rows: number of picture
     */

    nRows = data->length();
    nCols = column_name->length();
}

void Dataset::columns() const {
    int width = column_name->length();
    for (auto i = 0; i < width; i++) {
        std::cout << column_name->get(i);
        if (i < width-1) {
            std::cout << " ";
        }
    }
}

bool Dataset::drop(int axis, int index, std::string columns) {
    int width = column_name->length();
    int height = data->length();

    switch (axis) {
    // By row
    case 0: {
        if (index < height) {
            data->remove(index);
            return true;
        }
        else {
            return false;
        }
    }

    // By column
    case 1: {
        // Find the column's value = "columns"
        int column_index = -1;
        for (auto i = 0; i < height; i++) {
            if (columns.compare(column_name->get(i)) == 0) {
                column_index = i;
            }
        }
        if (column_index != -1) {
            for (auto i = 0; i < height; i++) {
                data->get(i)->remove(column_index);
            }
            column_name->remove(column_index);
            return true;
        }
        else {
            return false;
        }
    }
    default:
        return false;
    }
}

Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const
{
    auto width = column_name->length();
    auto height = data->length();
    
    // Reset "endRow" and "endCol"
    Dataset dataset;
    //startRow = (endRow == -1) ? 0 : startRow;
    endRow = (endRow == -1 || endRow > height) ? height-1 : endRow;
        
    //startCol = (endCol == -1) ? 0 : startCol;
    endCol = (endCol == -1 || endCol > width) ? width-1 : endCol;
        
    // get columns name 
    for (auto j = startCol; j < endCol+1; j++) {
        dataset.column_name->push_back(column_name->get(j));
    }

    // get the value.
    for (auto i = startRow; i < endRow+1; i++) {
        OList<int>* row = new OList<int>;
        for (auto j = startCol; j < endCol + 1; j++) {
            row->push_back(data->get(i)->get(j));
        }
        dataset.data->push_back(row);
    }

    return dataset;
}

List<List<int>*>* Dataset::getData() const
{
    return data;
}

int kNN::find_label(List<float>* Euclidean_row)
{
    //int label_size = y_train.getData()->length();
    ////List<int>* label = new OList<int>();
    //for (auto i = 0; i < label_size; i++) {
    //    label->push_back(y_train.getData()->get(i)->get(0));
    //}

    List<List<int>*>* label = y_train.getData();
    //std::cout << label->get(2)->get(0);
    //Euclidean_row->print();

 /*   Euclidean_row->print();
    std::cout << std::endl;
    label->print();
    std::cout << std::endl;*/

    int n_features = Euclidean_row->length();
    for (auto i = 0; i < n_features; i++) {
        for (auto j = i; j < n_features; j++) {
            if (Euclidean_row->get(j) < Euclidean_row->get(i)) {
                swap(Euclidean_row->get(j), Euclidean_row->get(i));
                swap(label->get(j), label->get(i));
            }
        }
    }

  /*  Euclidean_row->print();
    std::cout << std::endl;
    label->print();
    std::cout << std::endl;*/


    List<int>* top_k_label = new OList<int>();
    for (int i = 0; i < k; i++) {
        top_k_label->push_back(label->get(i)->get(0));
    }

    for (int i = 0; i < k; i++) {
        for (int j = i; j < k; j++) {
            if (top_k_label->get(j) < top_k_label->get(i)) {
                swap(top_k_label->get(j), top_k_label->get(i));
            }
        }
    }

    int flag = top_k_label->get(0);
    int pre_flag = flag;
    int pre_count = 0;
    int count = 0;
    for (int i = 1; i < k; i++) {
        if (flag == top_k_label->get(i)) {
            count++;
        }
        else {
            if (count > pre_count) {
                pre_flag = flag;
            }
            pre_count = count;
            count = 0;
        }
    }
    return pre_flag;
}

kNN::kNN(int k)
{
    this->k = k;
}

void kNN::fit(const Dataset& X_train, const Dataset& y_train)
{
    this->X_train = X_train;
    this->y_train = y_train;
}

Dataset kNN::predict(const Dataset& X_test)
{
    /*
        Using the Euclidean to compute the distance between 2 picture.

        Parameter:
        X_test: testing set. (included 784 features/ 1 picture)
        X_train: training set. (N picture)
        y_train: Labels of picture, match to X_train (1 columns, 199 row)
    */

    // Load the dataset

    // Init k value

    // Init the neccessary value
    int X_train_rows = 0, X_train_cols = 0,
        y_train_rows = 0, y_train_cols = 0,
        X_test_rows = 0, X_test_cols = 0;

    X_train.getShape(X_train_rows, X_train_cols);
    X_test.getShape(X_test_rows, X_test_cols);
    y_train.getShape(y_train_rows, y_train_cols);

    int sample = 0;
    List<List<float>*>* Euclidean_data = new OList<List<float>*>();
    List<List<int>*>* X_test_data = X_test.getData();
    List<List<int>*>* X_train_data = X_train.getData();
    List<List<int>*>* y_train_data = y_train.getData();

    // Loop picture 1 to N picture of X_train for computing the Euclidean distances
    while (sample < X_test_rows) {

        // Get 1 row in test dataset.
        List<int>* test_row = X_test_data->get(sample);
        List<float>* Euclidean_row = new OList<float>;

        // Initialize the List<float> for the distances of sample and all training 
        for (auto i = 0; i < X_train_rows; i++) {
           // Get 1 row in train dataset.
            List<int>* train_row = X_train_data->get(i);

            // Compute the Euclidean between 2 picture.
            float euclidean = Euclidean(test_row, train_row);
            Euclidean_row->push_back(euclidean);
        }
        Euclidean_data->push_back(Euclidean_row);
        sample++;
    }
    List<List<int>*>* row_predict = new OList<List<int>*>();
    for (auto i = 0; i < X_test_rows; i++) {
        List<int>* predict = new OList<int>();
        predict->push_back(find_label(Euclidean_data->get(i)));
        row_predict->push_back(predict);
    }
    List<std::string>* column_predict = new OList<std::string>;
    column_predict->push_back("predict");
    Dataset y_pred(row_predict, column_predict);
    return y_pred;
}

double kNN::score(const Dataset& y_test, const Dataset& y_pred)
{
    int size = y_test.getData()->length();
    List<List<int>*>* y_test_data = y_test.getData();
    List<List<int>*>* y_pred_data = y_pred.getData();

    int correct = 0;
    for (auto i = 0; i < size; i++) {
        if (y_test_data->get(i)->get(0) == y_pred_data->get(i)->get(0)) {
            correct++;
        }
    }
    return correct / size;
}

float Euclidean(List<int>* sample1, List<int>* sample2)
{
    /*
        Compute the Euclidean distance between 2 coordinates

        Euclidean = sqrt(sum( (x_i - y_i)^2 )) with i from 0 -> n_features.
    */
    int nFeatures = sample1->length();
    float euclidean = 0.0f;
    for (auto i = 0; i < nFeatures; i++) {
        float sub = static_cast<float>(sample1->get(i) - sample2->get(i));
        euclidean += pow(sub, 2);
    }
    euclidean = sqrt(euclidean);
    return euclidean;
}

void train_test_split(Dataset& X, Dataset& y, double test_size, Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test)
{
    int n_samples = 0, n_features = 0;
    y.getShape(n_samples, n_features);
    int a, b;

    int n_train_samples = n_samples * test_size;
    X_train = X.extract(0, n_train_samples, 0, -1);
    X_test = X.extract(n_train_samples + 1, -1, 0, -1);
    y_train = y.extract(0, n_train_samples, 0, -1);
    y_test = y.extract(n_train_samples + 1, -1, 0, -1);
}
