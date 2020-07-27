RGBD-RF
=======

This is a very basic *header-only* Random Forest library implementation.

To be able to use the library it is necessary to provide 4 things:

- Provide an implementation of a `rf::SplitCandidate`
- Something callable that generates an instance of a `rf::SplitCandidate`
- Wrap your data with a `rf::TrainSet`
- Provide the parameters for the training


## examples

- [UniversityOfWashingtonRGBDObjectDataset](examples/UniversityOfWashingtonRGBDObjectDataset): Using a Random Forest for RGB-D images classification.
