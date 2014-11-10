/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var tremppi_report = {
    Values: {
        reg_caption_c: "a) statistical properties of the selected parametrizations set, c) properties of the compared parametrizations set, b) difference between the two. " +
                + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
                + "Blunt edges denote positive and arrows describe the negative correlation value in the graphs a) and c). "
                + "In the graph b) the positive change in frequency is depicted by full edge, the negative by the dashed one. "
                + "The dotted edge is used when the difference in frequency is zero.",
        reg_caption_nonc: "a) statistical properties of the selected parametrizations set. " +
                + "F stands for frequency of occurrence of an edge in the given set, P for the Pearson correlation coefficient between the value of the regulator and the parameter value. "
                + "Blunt edges denote positive and arrows describe the negative correlation value. "
    },
    Regulatory : {} // Storing regulatory data
};