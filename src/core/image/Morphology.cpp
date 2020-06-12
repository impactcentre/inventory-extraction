/*******************************************************************************
 * 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#include "Morphology.hpp"

namespace impact_tr4 {

// Define basic structure elements:
/*static*/ const SE SE::RECT_SE_3x3 = SE(CV_SHAPE_ELLIPSE, 3, 3, -1, -1, 0x0);
/*static*/ const SE SE::RECT_SE_5x5 = SE(CV_SHAPE_ELLIPSE, 5, 5, -1, -1, 0x0);
/*static*/ const SE SE::RECT_SE_7x7 = SE(CV_SHAPE_ELLIPSE, 7, 7, -1, -1, 0x0);

/*static*/ const SE SE::CROSS_SE_3x3 = SE(CV_SHAPE_CROSS, 3, 3, -1, -1, 0x0);
/*static*/ const SE SE::CROSS_SE_5x5 = SE(CV_SHAPE_CROSS, 5, 5, -1, -1, 0x0);
/*static*/ const SE SE::CROSS_SE_7x7 = SE(CV_SHAPE_CROSS, 7, 7, -1, -1, 0x0);

/*static*/ const SE SE::CIRCLE_SE_3x3 = SE(CV_SHAPE_ELLIPSE, 3, 3, -1, -1, 0x0);
/*static*/ const SE SE::CIRCLE_SE_5x5 = SE(CV_SHAPE_ELLIPSE, 5, 5, -1, -1, 0x0);
/*static*/ const SE SE::CIRCLE_SE_7x7 = SE(CV_SHAPE_ELLIPSE, 7, 7, -1, -1, 0x0);

} // end of namespace impact_tr4
