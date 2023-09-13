#include "../include/tensor.h"

#include "internals/internal_tensor.hpp"
#include "internals/internal_buffer.hpp"
#include "internals/operations/internal_operations.hpp"

namespace net {

Tensor::Tensor(std::shared_ptr<internal::Tensor> tensor)
:   tensor_(tensor) {}

Tensor::Tensor(shape_type shape, bool gradient_requirement ) {
    tensor_ = std::make_shared<internal::Tensor>(shape);
    tensor_-> requires_gradient(gradient_requirement);
}

Tensor::Tensor(shape_type shape, storage_type data, bool gradient_requirement ) {
    tensor_ = std::make_shared<internal::Tensor>(shape);
    std::copy(data.begin(), data.end(), tensor_->begin());
    tensor_-> requires_gradient(gradient_requirement);
}

internal::Tensor* Tensor::internal() const {return tensor_.get(); }
internal::Tensor* Tensor::internal() { return tensor_.get(); }

void Tensor::backward(const Tensor& gradient) {
    tensor_-> backward(gradient.internal());
}

void Tensor::perform() const{
    tensor_-> forward();
}

Tensor::iterator Tensor::begin() { return tensor_->begin(); }
Tensor::iterator Tensor::end() { return tensor_->end(); }
Tensor::const_iterator Tensor::begin() const { return tensor_->begin(); }
Tensor::const_iterator Tensor::end() const { return tensor_->end(); }
Tensor::const_iterator Tensor::cbegin() const { return tensor_->cbegin(); }
Tensor::const_iterator Tensor::cend() const { return tensor_->cend(); }

Tensor operator + (const Tensor& first, const Tensor& second) {
    std::shared_ptr<internal::Tensor> expression = std::make_shared<internal::Addition>(
        first.internal(),
        second.internal()
    );

    internal::Buffer::add(expression);
    return Tensor(expression);
}

Tensor operator * (const Tensor& first, const Tensor& second) {
    std::shared_ptr<internal::Tensor> expression = std::make_shared<internal::Multiplication>(
        first.internal(),
        second.internal()
    );

    internal::Buffer::add(expression);
    return Tensor(expression);
}

Tensor matmul(const Tensor& first, const Tensor& second) {
    std::shared_ptr<internal::Tensor> expression = std::make_shared<internal::Matmul>(
        first.internal(),
        second.internal()
    );
    
    internal::Buffer::add(expression);
    return Tensor(expression);
}

} // namespace net

